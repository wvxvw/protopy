#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <alloca.h>

#include <Python.h>

#include "binparser.h"
#include "pyhelpers.h"


void free_state(PyObject* capsule) {
    if (capsule == Py_None) {
        return;
    }
    parse_state* state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    del(state->in);
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
        return Py_False;
    }
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
    state->in = nil;
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

    if (!PyArg_ParseTuple(args, "OOO", &capsule, &pytype, &factories)) {
        return NULL;
    }

    state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (state == NULL) {
        return NULL;
    }
    state->pytype = pytype;
    state->factories = factories;

    return Py_None;
}

int64_t state_get_available(parse_state* state) {
    return (int64_t)rope_length(state->in);
}

size_t state_read(parse_state* state, char* buf, size_t n) {
    list into;
    size_t result = rope_read(state->in, buf, n, &into);
    del(state->in);
    state->in = into;
    return result;
}

PyObject* state_get_field_pytype(parse_state* state) {
    if (state->is_field) {
        return state->pytype;
    }
    PyObject* key = Py_BuildValue("i", (int)state->field);
    PyObject* container_factory = PyDict_GetItem(state->factories, state->pytype);
    if (container_factory == NULL) {
        PyErr_SetString(PyExc_TypeError, "Nonexistent type");
        // invalid proto definition, trying to read non-existent type.
        return NULL;
    }
    PyObject* tmap = PyTuple_GetItem(container_factory, 3);
    return PyDict_GetItem(tmap, key);
}

vt_type_t state_get_field_type(parse_state* state) {
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
    if (factory == NULL) {
        factory = Py_None;
    }
    // TODO(olegs): We really need to do this just once, maybe at
    // state initialization time.
    PyObject* types = PyImport_ImportModule("protopy.types");
    PyObject* result = PyObject_CallMethod(types, "value_type", "OO", field_type, factory);
    return (vt_type_t)PyLong_AsLong(result);
}

vt_type_t state_get_field_repeated_type(parse_state* state) {
    PyObject* inner = PyList_GetItem(state->pytype, 0);
    PyObject* factory = PyDict_GetItem(state->factories, inner);
    if (factory == NULL) {
        factory = Py_None;
    }
    PyObject* types = PyImport_ImportModule("protopy.types");
    PyObject* result = PyObject_CallMethod(types, "value_type", "OO", inner, factory);
    return (vt_type_t)PyLong_AsLong(result);
}

size_t parse_varint_impl(parse_state* state, uint64_t value[2]) {
    // TODO(olegs): This can be made more efficient if we try to
    // pre-read more bytes.
    char* buf = alloca(sizeof(char) * 2);
    unsigned char current;
    size_t bytes_read = 0;
    size_t read = 0;
    size_t index = 0;

    while (state_get_available(state) > 0 && read < 16) {
        bytes_read = state_read(state, buf, 1);
        if (bytes_read == 0) {
            return read;
        }
        current = (unsigned char)buf[0];
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

size_t parse_zig_zag(parse_state* state, uint64_t value[2], bool* is_neg) {
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

size_t parse_varint(parse_state* state) {
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
        state->out = PyNumber_Or(low, high_shifted);
    } else {
        state->out = low;
    }
    if (sign) {
        state->out = PyNumber_Negative(state->out);
    }
    if (vt == vt_enum) {
        PyObject* enum_type = state_get_field_pytype(state);
        PyObject* factory = PyDict_GetItem(
            state->factories,
            enum_type);
        PyObject* ctor = PyTuple_GetItem(factory, 0);
        PyObject* result = PyObject_CallFunction(
            ctor,
            "OOO",
            enum_type,
            state->factories,
            state->out);
        state->out = result;
    }
    return parsed;
}

size_t parse_fixed_64(parse_state* state) {
#define FIXED_LENGTH 8
    char* buf = alloca((FIXED_LENGTH + 1) * sizeof(char));
    size_t read = 0;

    // TODO(olegs): Same as other reads: this must know how to give up
    // once the reading is no longer possible
    while (read < FIXED_LENGTH) {
        read += state_read(state, buf, FIXED_LENGTH);
    }
    unsigned long long val = 0;
    read = FIXED_LENGTH + 1;
    // ntohl only takes up to 32 bits
    while (read > 0) {
        read--;
        val <<= 8;
        val |= (unsigned long long)(unsigned char)buf[read];
    }
    if (state_get_field_type(state) == vt_fixed64) {
        state->out = PyLong_FromUnsignedLongLong(val);
    } else {
        state->out = PyLong_FromLongLong((long long)val);
    }
    return FIXED_LENGTH;
#undef FIXED_LENGTH
}

size_t parse_length_delimited(parse_state* state) {
    uint64_t value[2] = { 0, 0 };
    size_t parsed = parse_varint_impl(state, value);
    // No reason to care for high bits, we aren't expecting strings of
    // that length anyways.
    size_t length = (size_t)value[0];
    // TODO(olegs): Figure out what's the safe value to allocate on
    // stack and allocate on heap, if above the threshold.
    char* bytes = alloca(sizeof(char) * length);
    size_t read = 0;
    parse_state substate;

    while (read < length) {
        read += state_read(state, bytes + read, (size_t)length - read);
        // TODO(olegs): Maybe the interface to reading objects from
        // stream should be flexible enough to not block here until
        // the entire object is received.
        // TODO(olegs): This is a dangerous place because it may hang
        // if the socket closes in the of receiving a message.
    }
    switch (state_get_field_type(state)) {
        case vt_string:
            state->out = PyUnicode_FromStringAndSize(bytes, (Py_ssize_t)length);
            break;
        case vt_bytes:
            state->out = PyBytes_FromStringAndSize(bytes, (Py_ssize_t)length);
            break;
        case vt_message:
            substate.pos = 0;
            substate.in = nil;
            substate.factories = state->factories;
            substate.pytype = state_get_field_pytype(state);
            substate.is_field = false;
            state->out = parse_message(
                &substate,
                bytes,
                length);
            break;
        case vt_repeated:
            substate.pos = 0;
            substate.in = nil;
            substate.field = state->field;
            substate.factories = state->factories;
            substate.pytype = state_get_field_pytype(state);
            substate.is_field = false;
            state->out = parse_repeated(
                &substate,
                bytes,
                length);
            break;
        case vt_map:
            substate.pos = 0;
            substate.in = nil;
            substate.field = state->field;
            substate.factories = state->factories;
            substate.pytype = state_get_field_pytype(state);
            substate.is_field = false;
            state->out = parse_map(
                &substate,
                bytes,
                length);
            break;
        default:
            PyErr_SetString(
                PyExc_NotImplementedError,
                "Unknown length delimited type");
            state->out = Py_None;
    }
    return parsed + read;
}

size_t parse_start_group(parse_state* state) {
    PyErr_SetString(PyExc_NotImplementedError, "Proto v2 not supported");
    return 0;
}

size_t parse_end_group(parse_state* state) {
    PyErr_SetString(PyExc_NotImplementedError, "Proto v2 not supported");
    return 0;
}

size_t parse_fixed_32(parse_state* state) {
#define FIXED_LENGTH 4
    char* buf = alloca(FIXED_LENGTH * sizeof(char));
    size_t read = 0;

    // TODO(olegs): Same as other reads: this must know how to give up
    // once the reading is no longer possible
    while (read < FIXED_LENGTH) {
        read += state_read(state, buf, FIXED_LENGTH);
    }
    unsigned long val = 0;
    read = FIXED_LENGTH + 1;
    while (read > 0) {
        read--;
        val <<= 8;
        val |= (unsigned long)(unsigned char)buf[read];
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

PyObject* parse_message(parse_state* state, char* bytes, size_t len) {
    size_t i = 0;
    size_t j = 0;
    PyObject* dict = PyDict_New();
    PyObject* key;
    PyObject* existing;

    state->in = cons_str(bytes, len, state->in);

    while (i < len) {
        print_obj("parsing message field of: %s\n", state->pytype);
        printf("message bytes: %s\n", str(state->in));
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
        i += j;
        key = PyLong_FromUnsignedLong((unsigned long)state->field);
        existing = PyDict_GetItem(dict, key);
        if (existing) {
            if (PyList_CheckExact(existing)) {
                PyList_Append(existing, PyList_GetItem(state->out, 0));
                Py_DECREF(state->out);
            } else if (PyDict_CheckExact(existing)) {
                PyDict_Update(existing, state->out);
                Py_DECREF(state->out);
            } else {
                PyDict_SetItem(dict, key, state->out);
            }
        } else {
            PyDict_SetItem(dict, key, state->out);
        }
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
    PyObject* ctor = PyTuple_GetItem(factory, 0);
    PyObject* result = PyObject_CallFunction(
        ctor,
        "OOO",
        state->pytype,
        state->factories,
        dict);
    state->out = result;
    Py_INCREF(state->out);
    del(state->in);
    return result;
}

PyObject* parse_map(parse_state* state, char* bytes, size_t len) {
    PyObject* key_type = PyTuple_GetItem(state->pytype, 0);
    PyObject* value_type = PyTuple_GetItem(state->pytype, 1);
    PyObject* result = PyDict_New();
    parse_handler handler;

    state->in = cons_str(bytes, len, nil);
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

    if (state->field == 1) {
        key = state->out;
    } else {
        value = state->out;
    }

    select_handler(state, &handler);

    if (state->field == 1) {
        state->pytype = key_type;
    } else {
        state->pytype = value_type;
    }

    (*handler)(state);

    if (state->field == 1) {
        key = state->out;
    } else {
        value = state->out;
    }

    PyDict_SetItem(result, key, value);
    
    return result;
}

PyObject* parse_repeated(parse_state* state, char* bytes, size_t len) {
    size_t i = 0;
    size_t j = 0;
    PyObject* result = PyList_New(0);
    PyObject* subresult;
    vt_type_t rtype = state_get_field_repeated_type(state);

    Py_INCREF(result);
    state->in = cons_str(bytes, len, nil);

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
        while (i < len) {
            j = ph(state);
            if (j == 0) {
                break;
            }
            i += j;
            PyList_Append(result, state->out);
        }
    } else if (rtype == vt_string) {
        subresult = PyUnicode_FromStringAndSize(bytes, (Py_ssize_t)len);
        PyList_Append(result, subresult);
    } else if (rtype == vt_bytes) {
        subresult = PyBytes_FromStringAndSize(bytes, (Py_ssize_t)len);
        PyList_Append(result, subresult);
    } else {
        state->pytype = PyList_GetItem(state->pytype, 0);

        subresult = parse_message(state, bytes, len);
        PyList_Append(result, subresult);
    }
    return result;
}

size_t backtrack(parse_state* state) { return 0; }

size_t select_handler(parse_state* state, parse_handler* handler) {
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

size_t parse(parse_state* state) {
    parse_handler handler;
    size_t parsed = select_handler(state, &handler);
    return parsed + (*handler)(state);
}
