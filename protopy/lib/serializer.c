#include <Python.h>
#include <stdio.h>
#include <apr_general.h>

#include "list.h"
#include "serializer.h"
#include "binparser.h"
#include "pyhelpers.h"

// TODO(olegs): Maybe this can be replaced by APR arrays
void resize_buffer(wbuffer_t* buf, size_t n) {
    byte* old = buf->buf;
    if (n > buf->cap) {
        buf->buf = apr_palloc(buf->mp, buf->len + n);
    } else {
        buf->buf = apr_palloc(buf->mp, buf->cap * 2);
    }
    memcpy(buf->buf, old, buf->len);
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
        vu = PyLong_AsUnsignedLongLong(converted);
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
        resize_buffer(buf, buf->len + len - buf->cap);
    }
    memcpy(buf->buf + buf->len, bytes, len);
    buf->len += len;
}

void serialize_length_delimited(wbuffer_t* buf, PyObject* message) {
    char* bytes;

    if (PyUnicode_Check(message)) {
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
                PyExc_NotImplementedError,
                "Message serialization is not implemented yet.");
    }
}

void
serialize_repeated(
    wbuffer_t* buf,
    PyObject* message,
    byte wt,
    field_info_t* info,
    apr_hash_t* defs) {

    if (!PySequence_Check(message)) {
        PyErr_Format(
            PyExc_TypeError,
            "%A must implement sequence protocol", message);
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

    if (vt == vt_default || vt == vt_string || vt == vt_bytes) {
        char* key = NULL;
        factory_t* f = NULL;

        if (vt == vt_default) {
            key = bytes_cstr(info->pytype);
            f = apr_hash_get(defs, key, APR_HASH_KEY_STRING);
            if (!f) {
                PyErr_Format(
                    PyExc_TypeError,
                    "Don't know how to serialize %A as %s", message, key);
                return;
            }
        }
        for (i = 0; i < len; i++) {
            PyObject* pval = PySequence_GetItem(message, i);
            if (pval == Py_None) {
                continue;
            }
            serialize_varint_impl(buf, (unsigned long long)wt);
            if (vt == vt_default) {
                serialize_message(&subbuf, pval, f, defs, key);
                serialize_length_delimited_impl(buf, subbuf.buf, subbuf.len);
                subbuf.len = 0;
            } else {
                serialize_length_delimited(buf, pval);
            }
        }
        if (vt == vt_default) {
            free(key);
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
    byte wt,
    PyObject* pval,
    const byte* pytype,
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
        PyErr_Format(PyExc_TypeError, "%A must implement sequence protocol", message);
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
            char* key = bytes_cstr(info->pytype);

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
            wt |= (*field << 3);
            switch (vt) {
                case vt_repeated:
                    serialize_repeated(buf, pval, wt, info, defs);
                    break;
                case vt_map:
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
                Py_DECREF(pval);
                return;
            }
        } else {
            PyErr_Format(
                PyExc_TypeError,
                "Don't know how to serialize field %d of %A",
                i,
                message);
            Py_DECREF(pval);
            return;
        }
        Py_DECREF(pval);
    }
}

void proto_serialize_impl(
    wbuffer_t* buf,
    const byte* mtype,
    apr_hash_t* const defs,
    PyObject* message) {

    char* key = bytes_cstr(mtype);
    factory_t* f = apr_hash_get(defs, key, APR_HASH_KEY_STRING);

    if (!f) {
        vt_type_t vt = vt_builtin(mtype);
        if (vt != vt_default) {
            proto_serialize_builtin(buf, vt, defs, message);
        } else {
            PyErr_Format(PyExc_TypeError, "Unknown type: %s.", key);
            free(key);
            return;
        }
    } else {
        switch (f->vt_type) {
            case vt_enum:
                serialize_varint(buf, message, false);
                break;
            case vt_message:
                serialize_message(buf, message, f, defs, key);
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
                    "Expected container type: %s, but got %d.", key, f->vt_type);
                free(key);
                return;
        }
        free(key);
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
    
    byte* bmtype = cstr_bytes(mtype);
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
    proto_serialize_impl(&buf, bmtype, ht, message);
    
    if (PyErr_Occurred()) {
        free(bmtype);
        apr_pool_destroy(mp);
        return NULL;
    }
    PyObject* result = PyBytes_FromStringAndSize((char*)buf.buf, buf.len);
    free(bmtype);
    apr_pool_destroy(mp);
    return result;
}
