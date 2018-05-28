#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include <Python.h>

#include "binparser.h"
#include "pyhelpers.h"


void free_state(PyObject* capsule) {
    if (capsule == Py_None) {
        return;
    }
    parse_state* state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    free(state);
}

PyObject* state_ready(PyObject* self, PyObject* args) {
    PyObject* capsule;
    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    parse_state* state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (PyErr_Occurred()) {
        return NULL;
    }
    if (state->out == Py_None) {
        Py_INCREF(Py_False);
        return Py_False;
    }
    Py_INCREF(Py_True);
    return Py_True;
}

PyObject* state_result(PyObject* self, PyObject* args) {
    PyObject* capsule;
    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    parse_state* state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (PyErr_Occurred()) {
        return NULL;
    }
    return state->out;
}

PyObject* make_state(PyObject* self, PyObject* args) {
    parse_state* state = malloc(sizeof(parse_state));
    state->pos = 0;
    state->out = Py_None;
    state->is_field = false;
    return PyCapsule_New(state, NULL, free_state);
}

PyObject* state_set_factory(PyObject* self, PyObject* args) {
    parse_state* state;
    PyObject* capsule;
    PyObject* pytype;
    PyObject* factories;
    PyObject* builtins;

    if (!PyArg_ParseTuple(args, "OOOO", &capsule, &pytype, &factories, &builtins)) {
        return NULL;
    }

    state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (state == NULL) {
        return NULL;
    }
    state->pytype = pytype;
    state->factories = factories;
    state->builtin_types = builtins;

    Py_RETURN_NONE;
}

int64_t state_get_available(parse_state* const state) {
    return state->len - state->pos;
}

size_t state_read(parse_state* const state, unsigned char** buf, size_t n) {
    *buf = &state->in[state->pos];
    if (state->len >= state->pos + (int64_t)n) {
        state->pos += (int64_t)n;
        return n;
    }
    state->pos = state->len;
    return state->len - state->pos;
}

vt_type_t value_type(parse_state* const state, PyObject* pbtype, PyObject* factory) {
    if (PyList_CheckExact(pbtype)) {
        return vt_repeated;
    }
    if (PyTuple_CheckExact(pbtype)) {
        return vt_map;
    }
    PyObject* builtin = PyDict_GetItem(state->builtin_types, pbtype);
    if (builtin) {
        return (vt_type_t)PyLong_AsLong(builtin);
    }
    if (factory == NULL || factory == Py_None) {
        PyErr_Format(PyExc_TypeError, "No definition for type: %A", pbtype);
        return vt_error;
    }
    PyObject* kind = PyTuple_GetItem(factory, 0);
    return (vt_type_t)PyLong_AsLong(kind);
}

PyObject* state_get_field_pytype(parse_state* const state) {
    if (state->is_field) {
        return state->pytype;
    }
    PyObject* key = PyLong_FromSsize_t((Py_ssize_t)state->field);
    PyObject* container_factory = PyDict_GetItem(state->factories, state->pytype);
    if (container_factory == NULL) {
        Py_DECREF(key);
        PyErr_SetString(PyExc_TypeError, "Nonexistent type");
        // invalid proto definition, trying to read non-existent type.
        return NULL;
    }
    PyObject* tmap = PyTuple_GetItem(container_factory, 3);
    PyObject* result = PyDict_GetItem(tmap, key);
    if (!result) {
        PyErr_Format(
            PyExc_TypeError,
            "Encountered stray key: %A in message of type %A(%A)",
            key,
            state->pytype,
            tmap);
    }
    Py_DECREF(key);
    return result;
}

vt_type_t state_get_field_type(parse_state* const state) {
    if (PyList_CheckExact(state->pytype)) {
        return vt_repeated;
    }
    if (PyTuple_CheckExact(state->pytype)) {
        return vt_map;
    }
    PyObject* field_type;

    if (state->is_field) {
        field_type = state->pytype;
    } else {
        field_type = state_get_field_pytype(state);
        if (PyErr_Occurred()) {
            return vt_error;
        }
    }
    PyObject* factory = PyDict_GetItem(state->factories, field_type);
    return value_type(state, field_type, factory);
}

vt_type_t state_get_field_repeated_type(parse_state* const state) {
    PyObject* inner = PyList_GetItem(state->pytype, 0);
    PyObject* factory = PyDict_GetItem(state->factories, inner);
    return value_type(state, inner, factory);
}

size_t parse_varint_impl(parse_state* const state, uint64_t value[2]) {
    unsigned char* buf = NULL;
    unsigned char current;
    size_t bytes_read = 0;
    size_t read = 0;
    size_t index = 0;

    while (read < 16) {
        bytes_read = state_read(state, &buf, 1);
        if (bytes_read == 0) {
            PyErr_Format(
                PyExc_SyntaxError,
                "Didn't finish reading varint");
            return read;
        }
        current = buf[0];
        if (read == 7) {
            index = 1;
        }
        value[index] |= ((current & 0x7F) << (read * 7));
        read++;
        if ((current >> 7) == 0) {
            return read;
        }
    }
    return 0;
}

size_t parse_zig_zag(parse_state* const state, uint64_t value[2], bool* is_neg) {
    size_t parsed = parse_varint_impl(state, value);
    *is_neg = (value[0] & 1) == 1;
    uint64_t high = value[1];
    uint64_t low = value[0];
    low = ((high & 1) << 63) | (low >> 1);
    high >>= 1;
    value[0] = low;
    value[1] = high;
    return parsed;
}

bool is_signed_vt(vt_type_t vt) {
    return vt == vt_sing32 || vt == vt_sing64;
}

PyObject* tuple_from_dict(PyObject* ftype, PyObject* factory, PyObject* values) {
    PyObject* ttype = PyTuple_GetItem(factory, 1);
    PyObject* fmapping = PyTuple_GetItem(factory, 2);
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    Py_ssize_t arg_len = 0;
    Py_ssize_t field;
    PyObject* result;

    if (PyDict_Size(fmapping) == 0) {
        result = PyObject_Call(ttype, PyTuple_New(0), NULL);
        Py_INCREF(result);
        return result;
    }

    while (PyDict_Next(fmapping, &pos, &key, &value)) {
        field = (Py_ssize_t)PyLong_AsLong(value);
        if (field > arg_len) {
            arg_len = field;
        }
    }
    PyObject* args = PyTuple_New(arg_len + 1);

    pos = 0;
    while (pos < arg_len + 1) {
        Py_INCREF(Py_None);
        PyTuple_SetItem(args, pos, Py_None);
        pos++;
    }

    pos = 0;
    while (PyDict_Next(values, &pos, &key, &value)) {
        field = (Py_ssize_t)PyLong_AsLong(PyDict_GetItem(fmapping, key));
        PyTuple_SetItem(args, field, value);
    }
    result = PyObject_Call(ttype, args, NULL);
    if (result) {
        Py_INCREF(result);
    }
    return result;
}

PyObject* enum_from_dict(PyObject* ftype, PyObject* factory, PyObject* value) {
    PyObject* ttype = PyTuple_GetItem(factory, 1);
    PyObject* fmapping = PyTuple_GetItem(factory, 2);
    PyObject* args = PyDict_GetItem(fmapping, value);
    PyObject* result = PyObject_CallFunctionObjArgs(ttype, args, NULL);
    Py_INCREF(result);
    return result;
}

size_t parse_varint(parse_state* const state) {
    uint64_t value[2] = { 0, 0 };
    vt_type_t vt = state_get_field_type(state);
    bool sign = false;
    size_t parsed;
    
    if (is_signed_vt(vt)) {
        parsed = parse_zig_zag(state, value, &sign);
    } else {
        parsed = parse_varint_impl(state, value);
    }
    // TODO(olegs): We probably need to decref all of the below...
    PyObject* low = PyLong_FromUnsignedLongLong((unsigned long long)value[0]);
    
    if (value[1] > 0) {
        PyObject* high = PyLong_FromUnsignedLongLong((unsigned long long)value[1]);
        PyObject* shift = PyLong_FromLong(64L);
        PyObject* high_shifted = PyNumber_Lshift(high, shift);
        Py_DECREF(low);
        Py_DECREF(high);
        Py_DECREF(shift);
        state->out = PyNumber_Or(low, high_shifted);
    } else {
        state->out = low;
    }
    if (sign) {
        state->out = PyNumber_Negative(state->out);
    }
    if (vt == vt_enum) {
        PyObject* enum_type = state_get_field_pytype(state);
        PyObject* factory = PyDict_GetItem(state->factories, enum_type);
        state->out = enum_from_dict(enum_type, factory, state->out);
    }
    return parsed;
}

size_t parse_fixed_64(parse_state* const state) {
#define FIXED_LENGTH 8
    unsigned char* buf = NULL;
    size_t read = 0;

    state_read(state, &buf, FIXED_LENGTH);
    unsigned long long val = 0;
    read = FIXED_LENGTH;
    // ntohl only takes up to 32 bits
    while (read > 0) {
        read--;
        val <<= 8;
        val |= (unsigned long long)buf[read];
    }

    switch (state_get_field_type(state)) {
        case vt_fixed64:
            state->out = PyLong_FromUnsignedLongLong(val);
            break;
        case vt_double:
            state->out = PyFloat_FromDouble((double)val);
            break;
        default:
            state->out = PyLong_FromLongLong((long long)val);
            break;
    }
    Py_INCREF(state->out);
    return FIXED_LENGTH;
#undef FIXED_LENGTH
}

size_t parse_length_delimited(parse_state* const state) {
    uint64_t value[2] = { 0, 0 };
    size_t parsed = parse_varint_impl(state, value);
    // No reason to care for high bits, we aren't expecting strings of
    // that length anyways.
    size_t length = (size_t)value[0];
    unsigned char* bytes = NULL;
    size_t read = 0;
    parse_state substate;

    if (state_read(state, &bytes, length) != length) {
        return 0;
    }

    vt_type_t vt = state_get_field_type(state);
    switch (vt) {
        case vt_string:
            state->out = PyUnicode_FromStringAndSize((char*)bytes, (Py_ssize_t)length);
            Py_INCREF(state->out);
            break;
        case vt_bytes:
            state->out = PyBytes_FromStringAndSize((char*)bytes, (Py_ssize_t)length);
            Py_INCREF(state->out);
            break;
        case vt_message:
            substate.pos = 0;
            substate.in = bytes;
            substate.len = (int64_t)length;
            substate.factories = state->factories;
            substate.pytype = state_get_field_pytype(state);
            substate.is_field = false;
            substate.builtin_types = state->builtin_types;
            state->out = parse_message(&substate);
            Py_INCREF(state->out);
            break;
        case vt_repeated:
            substate.pos = 0;
            substate.in = bytes;
            substate.len = (int64_t)length;
            substate.field = state->field;
            substate.factories = state->factories;
            substate.pytype = state_get_field_pytype(state);
            substate.is_field = false;
            substate.builtin_types = state->builtin_types;
            state->out = parse_repeated(&substate);
            Py_INCREF(state->out);
            break;
        case vt_map:
            substate.pos = 0;
            substate.in = bytes;
            substate.len = (int64_t)length;
            substate.field = state->field;
            substate.factories = state->factories;
            substate.pytype = state_get_field_pytype(state);
            substate.is_field = false;
            substate.builtin_types = state->builtin_types;
            state->out = parse_map(&substate);
            Py_INCREF(state->out);
            break;
        default:
            if (!PyErr_Occurred()) {
                PyErr_SetString(
                    PyExc_NotImplementedError,
                    "Unknown length delimited type");
            }
            state->out = Py_None;
            Py_INCREF(state->out);
    }
    return parsed + read;
}

size_t parse_start_group(parse_state* const state) {
    PyErr_SetString(PyExc_NotImplementedError, "Proto v2 not supported");
    return 0;
}

size_t parse_end_group(parse_state* const state) {
    PyErr_SetString(PyExc_NotImplementedError, "Proto v2 not supported");
    return 0;
}

size_t parse_fixed_32(parse_state* const state) {
#define FIXED_LENGTH 4
    unsigned char* buf = NULL;
    size_t read = 0;

    state_read(state, &buf, FIXED_LENGTH);
    
    unsigned long val = 0;
    read = FIXED_LENGTH;
    while (read > 0) {
        read--;
        val <<= 8;
        val |= (unsigned long)buf[read];
    }
    if (state_get_field_type(state) == vt_fixed32) {
        state->out = PyLong_FromUnsignedLong(val);
    } else {
        if (val & 0x80000000) {
            state->out = PyLong_FromLong((long)val - 0xFFFFFFFF - 1);
        } else {
            state->out = PyLong_FromLong((long)val);
        }
    }
    return FIXED_LENGTH;
#undef FIXED_LENGTH
}

wiretype_t wiretype_of(vt_type_t vt) {
    switch (vt) {
        case vt_fixed64:
        case vt_sfixed64:
        case vt_double:
            return wt_fixed64;
        case vt_string:
        case vt_bytes:
        case vt_message:
        case vt_repeated:
            return wt_string;
        case vt_fixed32:
        case vt_sfixed32:
            return wt_fixed32;
        default:
            return wt_varint;
    }
}

bool is_scalar(vt_type_t vt) {
    switch (vt) {
        case vt_repeated:
        case vt_message:
        case vt_string:
        case vt_bytes:
            return false;
        default:
            return true;
    }
}

PyObject* parse_message(parse_state* const state) {
    int64_t i = 0;
    size_t j = 0;
    PyObject* dict = PyDict_New();
    PyObject* key;
    PyObject* existing;

    while (i < state->len) {
        j = parse(state);
        if (j == 0) {
            // TODO(olegs): We finished earlier than expected, need to
            // handle this differently.
            break;
        }
        if (PyErr_Occurred()) {
            Py_DECREF(dict);
            if (state->out != NULL
                && state->out != Py_None
                && state->out != Py_True
                && state->out != Py_False) {
                Py_DECREF(state->out);
            }
            return NULL;
        }
        i += (int64_t)j;
        key = PyLong_FromUnsignedLong((unsigned long)state->field);
        existing = PyDict_GetItem(dict, key);
        if (existing && state->out != NULL) {
            if (PyList_CheckExact(existing)) {
                PyList_Append(existing, PyList_GetItem(state->out, 0));
                Py_DECREF(state->out);
            } else if (PyDict_CheckExact(existing)) {
                PyDict_Update(existing, state->out);
                Py_DECREF(state->out);
            } else {
                PyDict_SetItem(dict, key, state->out);
                Py_DECREF(state->out);
            }
        } else if (state->out != NULL) {
            PyDict_SetItem(dict, key, state->out);
            Py_DECREF(state->out);
        }
        Py_DECREF(key);
    }
    PyObject* factory = PyDict_GetItem(state->factories, state->pytype);
    if (!factory) {
        PyErr_Format(
            PyExc_TypeError,
            "No definition for %A, (parsed: %A)",
            state->pytype,
            dict);
        return NULL;
    }
    vt_type_t ctor = (vt_type_t)PyLong_AsLong(PyTuple_GetItem(factory, 0));
    if (ctor == vt_message) {
        state->out = tuple_from_dict(state->pytype, factory, dict);
    } else {
        state->out = enum_from_dict(state->pytype, factory, dict);
    }
    Py_DECREF(dict);
    if (state->out != NULL) {
        Py_INCREF(state->out);
    }
    return state->out;
}

PyObject* parse_map(parse_state* const state) {
    PyObject* key_type = PyTuple_GetItem(state->pytype, 0);
    PyObject* value_type = PyTuple_GetItem(state->pytype, 1);
    PyObject* result = PyDict_New();
    parse_handler handler;

    select_handler(state, &handler);

    if (state->field == 1) {
        state->pytype = key_type;
    } else {
        state->pytype = value_type;
    }

    state->is_field = true;

    (*handler)(state);

    PyObject* key = Py_None;
    PyObject* value = Py_None;

    if (state->out != NULL) {
        if (state->field == 1) {
            key = state->out;
        } else {
            value = state->out;
        }
    } else {
        Py_DECREF(result);
        return NULL;
    }

    select_handler(state, &handler);
    if (state->field == 1) {
        state->pytype = key_type;
    } else {
        state->pytype = value_type;
    }

    (*handler)(state);
    
    if (state->out != NULL) {
        if (state->field == 1) {
            key = state->out;
        } else {
            value = state->out;
        }
    } else {
        Py_DECREF(result);
        return NULL;
    }

    PyDict_SetItem(result, key, value);
    return result;
}

PyObject* parse_repeated(parse_state* const state) {
    int64_t i = 0;
    size_t j = 0;
    PyObject* result = PyList_New(0);
    PyObject* subresult;
    vt_type_t rtype = state_get_field_repeated_type(state);

    if (is_scalar(rtype)) {
        parse_handler ph;

        switch (wiretype_of(rtype)) {
            case wt_varint:
                ph = &parse_varint;
                break;
            case wt_fixed32:
                ph = &parse_fixed_32;
                break;
            case wt_fixed64:
                ph = &parse_fixed_64;
                break;
            default:
                ph = &parse_length_delimited;
        }
        while (i < state->len) {
            j = ph(state);
            if (j == 0) {
                break;
            }
            i += (int64_t)j;
            PyList_Append(result, state->out);
            Py_DECREF(state->out);
        }
    } else if (rtype == vt_string) {
        subresult = PyUnicode_FromStringAndSize((char*)state->in, (Py_ssize_t)state->len);
        PyList_Append(result, subresult);
        Py_DECREF(subresult);
    } else if (rtype == vt_bytes) {
        subresult = PyBytes_FromStringAndSize((char*)state->in, (Py_ssize_t)state->len);
        PyList_Append(result, subresult);
        Py_DECREF(subresult);
    } else {
        state->pytype = PyList_GetItem(state->pytype, 0);

        subresult = parse_message(state);
        if (!subresult) {
            return NULL;
        }
        PyList_Append(result, subresult);
        Py_DECREF(subresult);
    }
    return result;
}

size_t backtrack(parse_state* const state) { return 0; }

size_t select_handler(parse_state* const state, parse_handler* handler) {
    if (state_get_available(state) == 0) {
        *handler = backtrack;
        return 0;
    }
    uint64_t value[2] = { 0, 0 };
    size_t parsed = parse_varint_impl(state, value);
    size_t wire_type = (size_t)(value[0] & 7);

    state->field = (size_t)(value[0] >> 3);

    switch (wire_type) {
        case 0:
            *handler = parse_varint;
            break;
        case 1:
            *handler = parse_fixed_64;
            break;
        case 2:
            *handler = parse_length_delimited;
            break;
        case 3:
            *handler = parse_start_group;
            break;
        case 4:
            *handler = parse_end_group;
            break;
        case 5:
            *handler = parse_fixed_32;
            break;
        case 6:
        case 7:
            *handler = backtrack;
            PyErr_SetString(
                PyExc_NotImplementedError,
                "Invalid encoding, wire_type 6 and 7 are not defined");
    }
    return parsed;
}

size_t parse(parse_state* const state) {
    parse_handler handler;
    size_t parsed = select_handler(state, &handler);
    return parsed + (*handler)(state);
}
