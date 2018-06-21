#include <stdio.h>
#include <Python.h>

#include "list.h"
#include "serializer.h"
#include "binparser.h"
#include "pyhelpers.h"

byte* serialize_varint(PyObject* message, bool sign) {
    PyObject* converted = PyNumber_Long(message);
    if (!converted) {
        return NULL;
    }
    size_t nbytes = 0;
    size_t i;
    byte* result;

    if (sign) {
        long long v = PyLong_AsLongLong(converted);
        long long vc = v;
        do {
            nbytes++;
            vc >>= 7;
        } while (vc);
        result = NULL;
    } else {
        unsigned long long v = PyLong_AsUnsignedLongLong(converted);
        unsigned long long vc = v;
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
            result[i] = (byte)(v & 0x7F);
            v >>= 7;
            if (v) {
                result[i] = result[i] | 0x80;
            }
            i++;
        }
    }
    return result;
}

byte* serialize_64_fixed(PyObject* message, bool sign) {
    return NULL;
}

byte* serialize_length_delimited(PyObject* message, bool unicode) {
    return NULL;
}

byte* serialize_32_fixed(PyObject* message, bool sign) {
    return NULL;
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
        case vt_fixed64:
        case vt_double:
            result = serialize_64_fixed(message, false);
            break;
        case vt_sfixed64:
            result = serialize_64_fixed(message, true);
            break;
        case vt_string:
            result = serialize_length_delimited(message, true);
            break;
        case vt_bytes:
            result = serialize_length_delimited(message, false);
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
