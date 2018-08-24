#include <Python.h>
#include <stdio.h>
#include <apr_general.h>

#include "helpers.h"
#include "serializer.h"
#include "binparser.h"
#include "pyhelpers.h"

// TODO(olegs): Maybe this can be replaced by APR arrays
void resize_buffer(wbuffer_t* buf, size_t n) {
    byte* old = buf->buf;
    size_t new_cap;
    if (n > buf->cap) {
        new_cap = buf->len + n;
    } else {
        new_cap = buf->cap * 2;
    }
    buf->buf = apr_palloc(buf->mp, new_cap);
    memmove(buf->buf, old, buf->len);
    buf->cap = new_cap;
}

void serialize_varint_impl(wbuffer_t* buf, unsigned long long vu) {
    unsigned long long vc = vu;
    size_t nbytes = 0;
    size_t i;

    do {
        nbytes++;
        vc >>= 7;
    } while (vc);
    i = 0;
    while (i < nbytes) {
        if (buf->len + i >= buf->cap) {
            resize_buffer(buf, 1);
        }
        buf->buf[buf->len + i] = (byte)(vu & 0x7F);
        vu >>= 7;
        if (vu) {
            buf->buf[buf->len + i] = (buf->buf[buf->len + i] | 0x80);
        }
        i++;
    }
    buf->len += i;
}

void serialize_varint(wbuffer_t* buf, PyObject* message, bool sign) {
    PyObject* converted = PyNumber_Long(message);
    if (!converted) {
        return;
    }
    long long v;
    unsigned long long vu;

    if (sign) {
        v = PyLong_AsLongLong(converted);
        if (v < 0) {
            v = -v;
            v <<= 1;
            v--;
        } else {
            v <<= 1;
        }
        vu = (unsigned long long)v;
    } else {
        vu = PyLong_AsUnsignedLongLongMask(converted);
    }
    Py_DECREF(converted);
    serialize_varint_impl(buf, vu);
}

void serialize_64_fixed_impl(wbuffer_t* buf, unsigned long long vu) {
    size_t i;

    for (i = 0; i < 8; i++) {
        if (buf->len + i >= buf->cap) {
            resize_buffer(buf, 1);
        }
        buf->buf[buf->len + i] = (byte)(vu & 0xFF);
        vu >>= 8;
    }
    buf->len += 8;
}

void serialize_64_fixed(wbuffer_t* buf, PyObject* message, bool sign) {
    PyObject* converted = PyNumber_Long(message);
    if (!converted) {
        return;
    }
    unsigned long long vu;
    if (sign) {
        vu = (unsigned long long)PyLong_AsLongLong(converted);
    } else {
        vu = PyLong_AsUnsignedLongLong(converted);
    }
    Py_DECREF(converted);

    if (PyErr_Occurred()) {
        return;
    }

    serialize_64_fixed_impl(buf, vu);
}

void serialize_double(wbuffer_t* buf, PyObject* message) {
    double val = PyFloat_AsDouble(message);
    if (PyErr_Occurred()) {
        return;
    }
    unsigned long long vu;
    memcpy(&vu, &val, 8);
    serialize_64_fixed_impl(buf, vu);
}

void serialize_length_delimited_impl(wbuffer_t* buf, const unsigned char* bytes, size_t len) {
    serialize_varint_impl(buf, (unsigned long long)len);

    if (buf->len + len >= buf->cap) {
        resize_buffer(buf, buf->len + len);
    }
    memcpy(buf->buf + buf->len, bytes, len);
    buf->len += len;
}

void serialize_length_delimited(wbuffer_t* buf, PyObject* message) {
    char* bytes;

    if (PyUnicode_Check(message)) {
        // This may error if message is a unicode, but cannot be
        // encoded as UTF-8.  Trying to implement some default policy
        // would incur a significant slowdown in average case.  So, we
        // expect users to supply only Unicode strings which can be
        // encoded as UTF-8.
        bytes = PyUnicode_AsUTF8(message);
    } else if (PyBytes_Check(message)) {
        bytes = PyBytes_AsString(message);
    } else {
        PyObject* uo = PyObject_Str(message);
        if (!uo) {
            return;
        }
        Py_DECREF(uo);
        bytes = PyUnicode_AsUTF8(uo);
    }
    if (PyErr_Occurred()) {
        return;
    }
    size_t len = strlen(bytes);
    serialize_length_delimited_impl(buf, (unsigned char*)bytes, len);
}

void serialize_32_fixed(wbuffer_t* buf, PyObject* message, bool sign) {
    PyObject* converted = PyNumber_Long(message);
    if (!converted) {
        return;
    }
    unsigned long vu;
    if (sign) {
        vu = (unsigned long)PyLong_AsLong(converted);
    } else {
        vu = PyLong_AsUnsignedLong(converted);
    }
    Py_DECREF(converted);

    if (PyErr_Occurred()) {
        return;
    }

    size_t i;
    for (i = 0; i < 4; i++) {
        if (buf->len + i >= buf->cap) {
            resize_buffer(buf, 1);
        }
        buf->buf[buf->len + i] = (byte)(vu & 0xFF);
        vu >>= 8;
    }
    buf->len += 4;
}

void proto_serialize_builtin(
    wbuffer_t* buf,
    vt_type_t vt,
    apr_hash_t* const defs,
    PyObject* message) {

    switch (vt) {
        case vt_int32:
        case vt_int64:
        case vt_uint32:
        case vt_uint64:
        case vt_bool:
        case vt_enum:
            serialize_varint(buf, message, false);
            break;
        case vt_sint32:
        case vt_sint64:
            serialize_varint(buf, message, true);
            break;
        case vt_double:
            serialize_double(buf, message);
            break;
        case vt_fixed64:
            serialize_64_fixed(buf, message, false);
            break;
        case vt_sfixed64:
            serialize_64_fixed(buf, message, true);
            break;
        case vt_string:
        case vt_bytes:
            serialize_length_delimited(buf, message);
            break;
        case vt_fixed32:
            serialize_32_fixed(buf, message, false);
            break;
        case vt_sfixed32:
            serialize_32_fixed(buf, message, true);
            break;
        default:
            PyErr_Format(
                PyExc_TypeError,
                "%A (vt_type: %d) is not a Protobuf built-in type.",
                message, vt);
    }
}

wiretype_t field_and_type(vt_type_t vt, size_t field) {
    byte wt = 0;

    switch (vt) {
        case vt_int32:
        case vt_int64:
        case vt_uint32:
        case vt_uint64:
        case vt_bool:
        case vt_enum:
        case vt_sint32:
        case vt_sint64:
            wt = 0;
            break;
        case vt_double:
        case vt_fixed64:
        case vt_sfixed64:
            wt = 1;
            break;
        case vt_fixed32:
        case vt_sfixed32:
            wt = 3;
            break;
        case vt_string:
        case vt_bytes:
            // TODO(olegs): Repeated fields, enums and maps
            // need special treatment.
        default:
            wt = 2;
            break;
    }
    return wt | (field << 3);
}

void
serialize_map(
    wbuffer_t* buf,
    PyObject* message,
    wiretype_t wt,
    field_info_t* info,
    apr_hash_t* defs) {

    // TODO(olegs): Add fast path for dictionaries
    if (!PyMapping_Check(message)) {
        PyErr_Format(
            PyExc_TypeError,
            "%A must be a dictionary", message);
        return;
    }
    vt_type_t kvt = info->extra_type_info.pair.key;
    vt_type_t vvt = info->extra_type_info.pair.val;
    // TODO(olegs): Maybe we can thread these extra buffers when
    // calling this function so that we can allocate less memory.
    wbuffer_t subbuf = {
        apr_palloc(buf->mp, buf->page_size),
        buf->page_size,
        0,
        buf->page_size,
        buf->mp
    };
    wbuffer_t mesbuf = {
        apr_palloc(buf->mp, buf->page_size),
        buf->page_size,
        0,
        buf->page_size,
        buf->mp
    };

    const char* key = info->extra_type_info.pair.pyval;
    factory_t* f = NULL;

    if (vvt == vt_default || vvt == vt_message || vvt == vt_enum) {
        f = apr_hash_get(defs, key, APR_HASH_KEY_STRING);
        if (!f) {
            PyErr_Format(
                PyExc_TypeError,
                "Don't know how to serialize %A as %s", message, key);
            return;
        }
        if (f->vt_type != vt_message && f->vt_type != vt_enum) {
            PyErr_Format(
                PyExc_TypeError,
                "Expected %A to be a message or enum of type %s",
                message, key);
            return;
        }
        vvt = f->vt_type;
    }

    Py_ssize_t len = PyMapping_Size(message);
    PyObject* items = PyMapping_Items(message);
    Py_ssize_t i;
    PyObject* pykey;
    PyObject* pyvalue;
    PyObject* pypair;
    wiretype_t iwt;
    const char* pytype = info->extra_type_info.pair.pyval;

    for (i = 0; i < len; i++) {
        pypair = PyList_GetItem(items, i);
        pyvalue = PyTuple_GetItem(pypair, 1);
        if (pyvalue == Py_None) {
            continue;
        }
        pykey = PyTuple_GetItem(pypair, 0);
        serialize_varint_impl(buf, (unsigned long long)wt);
        iwt = field_and_type(kvt, 1);
        serialize_varint_impl(&subbuf, (unsigned long long)iwt);
        proto_serialize_builtin(&subbuf, kvt, defs, pykey);
        if (PyErr_Occurred()) {
            break;
        }
        iwt = field_and_type(vvt, 2);
        serialize_varint_impl(&subbuf, (unsigned long long)iwt);

        if (vvt == vt_string || vvt == vt_bytes || is_scalar(vvt)) {
            proto_serialize_builtin(&subbuf, vvt, defs, pyvalue);
        } else {
            serialize_message(&mesbuf, pyvalue, f, defs, pytype);
            serialize_length_delimited_impl(&subbuf, mesbuf.buf, mesbuf.len);
            mesbuf.len = 0;
        }
        if (PyErr_Occurred()) {
            break;
        }
        serialize_length_delimited_impl(buf, subbuf.buf, subbuf.len);
        subbuf.len = 0;
    }
    Py_DECREF(items);
}

void
serialize_repeated(
    wbuffer_t* buf,
    PyObject* message,
    wiretype_t wt,
    field_info_t* info,
    apr_hash_t* defs) {

    if (!PySequence_Check(message)) {
        PyErr_Format(
            PyExc_TypeError,
            "%A must implement sequence protocol (should be of type %s)",
            message,
            info->pytype);
        return;
    }
    Py_ssize_t len = PySequence_Size(message);
    Py_ssize_t i;

    vt_type_t vt = vt_builtin(info->pytype);
    wbuffer_t subbuf = {
        apr_palloc(buf->mp, buf->page_size),
        buf->page_size,
        0,
        buf->page_size,
        buf->mp
    };

    const char* key = info->pytype;
    factory_t* f = apr_hash_get(defs, key, APR_HASH_KEY_STRING);

    if (vt == vt_default) {
        if (info->extra_type_info.elt != vt_default) {
            vt = info->extra_type_info.elt;
        } else {
            if (!f) {
                PyErr_Format(
                    PyExc_TypeError,
                    "Don't know how to serialize %A as %s", message, key);
                return;
            }
            vt = f->vt_type;
        }
    }

    if (vt == vt_message || vt == vt_string || vt == vt_bytes) {
        for (i = 0; i < len; i++) {
            PyObject* pval = PySequence_GetItem(message, i);
            if (pval == Py_None) {
                continue;
            }
            serialize_varint_impl(buf, (unsigned long long)wt);
            if (vt == vt_message) {
                serialize_message(&subbuf, pval, f, defs, key);
                serialize_length_delimited_impl(buf, subbuf.buf, subbuf.len);
                subbuf.len = 0;
            } else {
                serialize_length_delimited(buf, pval);
            }
        }
    } else {
        for (i = 0; i < len; i++) {
            PyObject* pval = PySequence_GetItem(message, i);
            proto_serialize_impl(&subbuf, info->pytype, defs, pval);
        }
        serialize_varint_impl(buf, (unsigned long long)wt);
        serialize_length_delimited_impl(buf, subbuf.buf, subbuf.len);
    }
}

void
serialize_submessage(
    wbuffer_t* const buf,
    wiretype_t wt,
    PyObject* pval,
    const char* pytype,
    apr_hash_t* defs) {

    wbuffer_t subbuf = {
        apr_palloc(buf->mp, buf->page_size),
        buf->page_size,
        0,
        buf->page_size,
        buf->mp
    };
    serialize_varint_impl(buf, (unsigned long long)wt);
    proto_serialize_impl(&subbuf, pytype, defs, pval);
    if (PyErr_Occurred()) {
        Py_DECREF(pval);
        return;
    }
    serialize_length_delimited_impl(buf, subbuf.buf, subbuf.len);
}

// FIXME(olegs): This function is too long.  Refactor.
void serialize_message(
    wbuffer_t* buf,
    PyObject* message,
    factory_t* f,
    apr_hash_t* defs,
    const char* pytype) {
    // IMPORTANT: We assume that whatever the implementation of messages is
    // it allows us to call __getitem__ on these object in order to get the
    // value we need to encode.
    if (!PySequence_Check(message)) {
        PyErr_Format(
            PyExc_TypeError,
            "%A must implement sequence protocol (should be of type %s)",
            message,
            pytype);
        return;
    }

    apr_hash_t* inverse = apr_hash_make(buf->mp);
    apr_hash_t* fields = apr_hash_make(buf->mp);
    apr_hash_index_t* hi;
    field_info_t* val;
    const size_t* key;

    for (hi = apr_hash_first(buf->mp, f->mapping); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, (const void**)&key, NULL, (void**)&val);
        apr_hash_set(inverse, &val->n, sizeof(size_t), val);
        apr_hash_set(fields, &val->n, sizeof(size_t), key);
    }
    Py_ssize_t len = PySequence_Size(message);
    Py_ssize_t i;

    for (i = 0; i < len; i++) {
        PyObject* pval = PySequence_GetItem(message, i);
        if (pval == Py_None) {
            continue;
        }
        field_info_t* info = apr_hash_get(inverse, &i, sizeof(size_t));
        size_t* field = apr_hash_get(fields, &i, sizeof(size_t));

        if (info) {
            vt_type_t vt = vt_default;
            const char* key = info->pytype;

            if (info->vt_type != vt_default) {
                vt = info->vt_type;
            } else {
                factory_t* ff = apr_hash_get(defs, key, APR_HASH_KEY_STRING);
                if (ff) {
                    vt = ff->vt_type;
                } else {
                    vt = vt_builtin(info->pytype);
                }
                info->vt_type = vt;
            }
            wiretype_t wt = field_and_type(vt, *field);
            switch (vt) {
                case vt_repeated:
                    serialize_repeated(buf, pval, wt, info, defs);
                    break;
                case vt_map:
                    serialize_map(buf, pval, wt, info, defs);
                    break;
                case vt_message:
                    serialize_submessage(buf, wt, pval, info->pytype, defs);
                    break;
                default:
                    serialize_varint_impl(buf, (unsigned long long)wt);
                    proto_serialize_impl(buf, info->pytype, defs, pval);
                    break;
            }
            if (PyErr_Occurred()) {
                return;
            }
        } else {
            PyErr_Format(
                PyExc_TypeError,
                "Don't know how to serialize field %d of %A",
                i,
                message);
            return;
        }
    }
}

void proto_serialize_impl(
    wbuffer_t* buf,
    const char* mtype,
    apr_hash_t* const defs,
    PyObject* message) {

    factory_t* f = apr_hash_get(defs, mtype, APR_HASH_KEY_STRING);

    if (!f) {
        vt_type_t vt = vt_builtin(mtype);
        if (vt != vt_default) {
            proto_serialize_builtin(buf, vt, defs, message);
        } else {
            PyErr_Format(PyExc_TypeError, "Unknown type: %s.", mtype);
            return;
        }
    } else {
        switch (f->vt_type) {
            case vt_enum:
                serialize_varint(buf, message, false);
                break;
            case vt_message:
                serialize_message(buf, message, f, defs, mtype);
                break;
            case vt_repeated:
                break;
            case vt_map:
                break;
            case vt_error:
            case vt_default:
            default:
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "Expected container type: %s, but got %d.", mtype, f->vt_type);
                return;
        }
    }
}

PyObject* proto_serialize(PyObject* self, PyObject* args) {
    PyObject* message;
    char* mtype;
    PyObject* defs;
    Py_ssize_t buf_size;

    if (!PyArg_ParseTuple(args, "OyOn", &message, &mtype, &defs, &buf_size)) {
        return NULL;
    }

    apr_hash_t* ht = (apr_hash_t*)PyCapsule_GetPointer(defs, NULL);
    if (!ht) {
        PyErr_Format(PyExc_ValueError, "Couldn't access definitions hash table");
        return NULL;
    }
    
    apr_pool_t* mp;

    if (apr_pool_create(&mp, NULL) != APR_SUCCESS) {
        PyErr_SetString(PyExc_TypeError, "Couldn't create memory pool");
        return NULL;
    }

    wbuffer_t buf;
    buf.buf = apr_palloc(mp, buf_size * sizeof(byte));
    buf.cap = buf_size;
    buf.len = 0;
    buf.mp = mp;
    buf.page_size = buf_size * sizeof(byte);
    proto_serialize_impl(&buf, mtype, ht, message);
    
    if (PyErr_Occurred()) {
        apr_pool_destroy(mp);
        return NULL;
    }
    char* res = malloc(buf.len * sizeof(char));
    memcpy(res, buf.buf, buf.len);
    PyObject* result = PyBytes_FromStringAndSize(res, buf.len);
    apr_pool_destroy(mp);
    return result;
}
