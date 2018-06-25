#include <stdio.h>
#include <Python.h>
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
        Py_DECREF(converted);
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
        Py_DECREF(converted);
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
    serialize_varint_impl(buf, (unsigned long long)len);

    if (buf->len + len >= buf->cap) {
        resize_buffer(buf, buf->len + len - buf->cap);
    }
    memcpy(buf->buf + buf->len, bytes, len);
    buf->len += len;
}

void serialize_32_fixed(wbuffer_t* buf, PyObject* message, bool sign) {
    PyObject* converted = PyNumber_Long(message);
    if (!converted) {
        Py_DECREF(converted);
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

void serialize_message(
    wbuffer_t* buf,
    PyObject* message,
    factory_t* f,
    const char* pytype,
    apr_pool_t* mp) {
    // IMPORTANT: We assume that whatever the implementation of messages is
    // it allows us to call __getitem__ on these object in order to get the
    // value we need to encode.
    if (!PySequence_Check(message)) {
        PyErr_Format(PyExc_TypeError, "%A must implement sequence protocol", message);
        return;
    }

    apr_hash_t* inverse = apr_hash_make(mp);
    apr_hash_index_t* hi;
    field_info_t* val;
    const size_t* key;

    for (hi = apr_hash_first(mp, f->mapping); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, (const void**)&key, NULL, (void**)&val);
        apr_hash_set(inverse, &val->n, sizeof(size_t), val);
    }
    Py_ssize_t len = PySequence_Size(message);
    Py_ssize_t i;

    for (i = 0; i < len; i++) {
        PyObject* pval = PySequence_GetItem(message, i);
        
        Py_DECREF(pval);
    }
}

void proto_serialize_impl(
    wbuffer_t* buf,
    const byte* mtype,
    apr_hash_t* const defs,
    PyObject* message,
    apr_pool_t* mp) {

    vt_type_t vt = vt_builtin(mtype);

    if (vt != vt_default) {
        proto_serialize_builtin(buf, vt, defs, message);
    } else {
        char* key = bytes_cstr(mtype);
        factory_t* f = apr_hash_get(defs, key, APR_HASH_KEY_STRING);
        if (!f) {
            PyErr_Format(PyExc_TypeError, "Unknown type: %s.", key);
            free(key);
            return;
        }
        switch (f->vt_type) {
            case vt_enum:
                break;
            case vt_message:
                serialize_message(buf, message, f, key, mp);
                break;
            case vt_repeated:
                break;
            case vt_map:
                break;
            case vt_error:
                break;
            case vt_default:
                break;
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
    proto_serialize_impl(&buf, bmtype, ht, message, mp);
    
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
