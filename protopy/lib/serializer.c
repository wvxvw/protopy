#include <stdio.h>
#include <Python.h>

#include "list.h"
#include "serializer.h"
#include "binparser.h"
#include "pyhelpers.h"

byte* serialize_varint_impl(unsigned long long vu) {
    unsigned long long vc = vu;
    size_t nbytes = 0;
    byte* result;
    size_t i;

    do {
        nbytes++;
        vc >>= 7;
    } while (vc);
    result = malloc((2 + nbytes) * sizeof(byte));
    result[0] = (byte)(nbytes >> 8);
    result[1] = (byte)(nbytes & 0xFF);
    i = 2;
    nbytes += 2;
    while (i < nbytes) {
        result[i] = (byte)(vu & 0x7F);
        vu >>= 7;
        if (vu) {
            result[i] = result[i] | 0x80;
        }
        i++;
    }
    return result;
}

byte* serialize_varint(PyObject* message, bool sign) {
    PyObject* converted = PyNumber_Long(message);
    if (!converted) {
        Py_DECREF(converted);
        return NULL;
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
    return serialize_varint_impl(vu);
}

byte* serialize_64_fixed_impl(unsigned long long vu) {
    byte* result = malloc(10 * sizeof(byte));
    result[0] = 0;
    result[1] = 8;
    size_t i;

    for (i = 2; i < 10; i++) {
        result[i] = (byte)(vu & 0xFF);
        vu >>= 8;
    }
    return result;
}

byte* serialize_64_fixed(PyObject* message, bool sign) {
    PyObject* converted = PyNumber_Long(message);
    if (!converted) {
        Py_DECREF(converted);
        return NULL;
    }
    unsigned long long vu;
    if (sign) {
        vu = (unsigned long long)PyLong_AsLongLong(converted);
    } else {
        vu = PyLong_AsUnsignedLongLong(converted);
    }
    Py_DECREF(converted);

    if (PyErr_Occurred()) {
        return NULL;
    }

    return serialize_64_fixed_impl(vu);
}

byte* serialize_double(PyObject* message) {
    double val = PyFloat_AsDouble(message);
    if (PyErr_Occurred()) {
        return NULL;
    }
    unsigned long long vu;
    memcpy(&vu, &val, 8);
    return serialize_64_fixed_impl(vu);
}

byte* serialize_length_delimited(PyObject* message) {
    byte* result;
    if (PyUnicode_Check(message)) {
        result = cstr_bytes(PyUnicode_AsUTF8(message));
    } else if (PyBytes_Check(message)) {
        result = cstr_bytes(PyBytes_AsString(message));
    } else {
        PyObject* uo = PyObject_Str(message);
        if (!uo) {
            return NULL;
        }
        Py_DECREF(uo);
        result = cstr_bytes(PyUnicode_AsUTF8(uo));
    }
    size_t len = str_size(result);
    byte* sz = serialize_varint_impl((unsigned long long)len);
    size_t sz_len = str_size(sz);
    size_t total = len + sz_len;
    byte* final = malloc((2 + total) * sizeof(byte));
    final[0] = total >> 8;
    final[1] = total & 0xFF;
    memcpy(final + 2, sz + 2, sz_len);
    memcpy(final + 2 + sz_len, result + 2, len);
    return final;
}

byte* serialize_32_fixed(PyObject* message, bool sign) {
    PyObject* converted = PyNumber_Long(message);
    if (!converted) {
        Py_DECREF(converted);
        return NULL;
    }
    unsigned long vu;
    if (sign) {
        vu = (unsigned long)PyLong_AsLong(converted);
    } else {
        vu = PyLong_AsUnsignedLong(converted);
    }
    Py_DECREF(converted);

    if (PyErr_Occurred()) {
        return NULL;
    }

    byte* result = malloc(6 * sizeof(byte));
    result[0] = 0;
    result[1] = 4;
    size_t i;

    for (i = 2; i < 6; i++) {
        result[i] = (byte)(vu & 0xFF);
        vu >>= 8;
    }
    return result;
}

byte* proto_serialize_builtin(vt_type_t vt, apr_hash_t* const defs, PyObject* message) {
    byte* result;

    switch (vt) {
        case vt_int32:
        case vt_int64:
        case vt_uint32:
        case vt_uint64:
        case vt_bool:
        case vt_enum:
            result = serialize_varint(message, false);
            break;
        case vt_sint32:
        case vt_sint64:
            result = serialize_varint(message, true);
            break;
        case vt_double:
            result = serialize_double(message);
            break;
        case vt_fixed64:
            result = serialize_64_fixed(message, false);
            break;
        case vt_sfixed64:
            result = serialize_64_fixed(message, true);
            break;
        case vt_string:
        case vt_bytes:
            result = serialize_length_delimited(message);
            break;
        case vt_fixed32:
            result = serialize_32_fixed(message, false);
            break;
        case vt_sfixed32:
            result = serialize_32_fixed(message, true);
            break;
        default:
            PyErr_Format(
                PyExc_NotImplementedError,
                "Message serialization is not implemented yet.");
            return NULL;
    }
    return result;
}

PyObject* proto_serialize_impl(const byte* mtype, apr_hash_t* const defs, PyObject* message) {
    vt_type_t vt = vt_builtin(mtype);
    PyObject* result = NULL;
    if (vt != vt_default) {
        byte* payload = proto_serialize_builtin(vt, defs, message);

        if (PyErr_Occurred()) {
            if (payload != NULL) {
                free(payload);
            }
            return NULL;
        }
        if (payload) {
            result = PyBytes_FromStringAndSize(
                (const char*)(payload + 2),
                str_size(payload));
        } else {
            PyErr_Format(
                PyExc_NotImplementedError,
                "Message serialization is not implemented yet.");
            
        }
    } else {
        PyErr_Format(
            PyExc_NotImplementedError,
            "Message serialization is not implemented yet.");
    }
    return result;
}

PyObject* proto_serialize(PyObject* self, PyObject* args) {
    PyObject* message;
    char* mtype;
    PyObject* defs;

    if (!PyArg_ParseTuple(args, "OyO", &message, &mtype, &defs)) {
        return NULL;
    }

    apr_hash_t* ht = (apr_hash_t*)PyCapsule_GetPointer(defs, NULL);
    if (!ht) {
        PyErr_Format(PyExc_ValueError, "Couldn't access definitions hash table");
        return NULL;
    }
    
    byte* bmtype = cstr_bytes(mtype);
    PyObject* result = proto_serialize_impl(bmtype, ht, message);
    free(bmtype);
    return result;
}
