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
    printf("freeing the state %p\n", state);
    free(state);
    printf("state freed\n");
}

PyObject* state_ready(PyObject* self, PyObject* args) {
    printf("testing if state is ready\n");
    PyObject* capsule;
    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    printf("state_ready: all arguments parsed\n");
    parse_state* state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (PyErr_Occurred()) {
        return NULL;
    }
    printf("state_ready: capsule extracted\n");
    print_obj("state->out was: %s\n", state->out);
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
    printf("allocating new state %p\n", state);
    return PyCapsule_New(state, NULL, free_state);
}

PyObject* state_set_factory(PyObject* self, PyObject* args) {
    parse_state* state;
    PyObject* capsule;
    PyObject* pytype;
    PyObject* descriptions;
    PyObject* factories;

    if (!PyArg_ParseTuple(args, "OOOO", &capsule, &pytype, &descriptions, &factories)) {
        return NULL;
    }

    state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (state == NULL) {
        return NULL;
    }
    state->description = descriptions;
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

vt_type_t state_get_field_type(parse_state* state) {
    PyObject* key = Py_BuildValue("i", (int)state->field);
    print_obj("1. state_get_field_type: %s\n", key);
    PyObject* container_factory = PyDict_GetItem(state->factories, state->pytype);
    print_obj("2. state_get_field_type: %s\n", container_factory);
    print_obj("3. state_get_field_type: %s\n", state->factories);
    print_obj("4. state_get_field_type: %s\n", state->pytype);
    PyObject* tmap = PyTuple_GetItem(container_factory, 3);
    print_obj("5. state_get_field_type: %s\n", tmap);
    PyObject* field_type = PyDict_GetItem(tmap, key);
    print_obj("6. state_get_field_type: %s\n", field_type);
    PyObject* factory = PyDict_GetItem(state->factories, field_type);
    print_obj("7. state_get_field_type: %s\n", factory);
    if (factory == NULL) {
        factory = Py_None;
    }
    PyObject* types = PyImport_ImportModule("protopy.types");
    PyObject* result = PyObject_CallMethod(types, "value_type", "OO", field_type, factory);
    print_obj("8. state_get_field_type: %s\n", result);
    PyErr_Print();
    return (vt_type_t)PyLong_AsLong(result);
}

size_t parse_varint_impl(parse_state* state, int64_t value[2]) {
    // TODO(olegs): This can be made more efficient if we try to
    // pre-read more bytes.
    char* buf = alloca(sizeof(char) * 2);
    char current;
    size_t bytes_read = 0;
    size_t read = 0;
    size_t index = 0;

    while (state_get_available(state) > 0 && read < 16) {
        bytes_read = state_read(state, buf, 1);
        if (bytes_read == 0) {
            return read;
        }
        current = buf[0];
        if (read == 7) {
            index = 1;
        }
        value[index] |= ((current & 0x7F) << (read * 8));
        read++;
        if ((current >> 7) == 0) {
            return read;
        }
    }
    return 0;
}

size_t parse_zig_zag(parse_state* state, int64_t value[2], bool* is_neg) {
    size_t parsed = parse_varint_impl(state, value);
    *is_neg = (value[0] & 1) == 1;
    int64_t high = value[1];
    int64_t low = value[0];
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
    int64_t value[2] = { 0, 0 };
    vt_type_t vt = state_get_field_type(state);
    bool sign = false;
    size_t parsed;
    
    if (is_signed_vt(vt)) {
        parsed = parse_zig_zag(state, value, &sign);
    } else {
        parsed = parse_varint_impl(state, value);
    }
    PyObject* low = PyLong_FromUnsignedLongLong(value[0]);
    
    if (value[1] > 0) {
        PyObject* high = PyLong_FromUnsignedLongLong(value[1]);
        PyObject* shift = PyLong_FromLong(64L);
        PyObject* high_shifted = PyNumber_Lshift(high, shift);
        state->out = PyNumber_Or(low, high_shifted);
    } else {
        state->out = low;
    }
    if (sign) {
        state->out = PyNumber_Negative(state->out);
    }
    printf("parsed varint ");
    PyObject_Print(state->out, stdout, 0);
    printf("\n");
    if (vt == vt_enum) {
        // TODO(olegs): We'd need to find an enum instance for
        // corresponding to this number.
    }
    return parsed;
}

size_t parse_fixed_64(parse_state* state) {
#define FIXED_LENGTH 8
    char* buf = alloca(FIXED_LENGTH * sizeof(char));
    size_t read = 0;

    // TODO(olegs): Same as other reads: this must know how to give up
    // once the reading is no longer possible
    while (read < FIXED_LENGTH) {
        read += state_read(state, buf, FIXED_LENGTH);
    }
    if (state_get_field_type(state) == vt_fixed64) {
        // TODO(olegs): I'm not sure about the endiannes.
        state->out = PyLong_FromUnsignedLongLong((uint64_t)(*buf));
    } else {
        state->out = PyLong_FromLongLong((int64_t)(*buf));
    }
    return FIXED_LENGTH;
#undef FIXED_LENGTH
}

size_t parse_length_delimited(parse_state* state) {
    int64_t value[2] = { 0, 0 };
    size_t parsed = parse_varint_impl(state, value);
    // No reason to care for high bits, we aren't expecting strings of
    // that length anyways.
    size_t length = (size_t)value[0];
    printf("parse_length_delimited length(%zu)\n", length);
    // TODO(olegs): Figure out what's the safe value to allocate on
    // stack and allocate on heap, if above the threshold.
    char* bytes = alloca(sizeof(char) * length);
    size_t read = 0;

    while (read < length) {
        read += state_read(state, bytes + read, (size_t)length - read);
        // TODO(olegs): Maybe the interface to reading objects from
        // stream should be flexible enough to not block here until
        // the entire object is received.
        // TODO(olegs): This is a dangerous place because it may hang
        // if the socket closes in the of receiving a message.
    }
    printf("parse_length_delimited type(%d)\n", (int)state_get_field_type(state));
    switch (state_get_field_type(state)) {
        case vt_default:
        case vt_string:
            printf("creating python string\n");
            state->out = PyBytes_FromStringAndSize(bytes, (Py_ssize_t)length);
            break;
        case vt_bytes:
            state->out = PyUnicode_FromStringAndSize(bytes, (Py_ssize_t)length);
            break;
        case vt_message:
            printf("encountered inner message: %zu\n", length);
            state->out = parse_message(
                state,
                bytes,
                length);
            break;
        case vt_repeated:
            state->out = parse_repeated(
                state,
                bytes,
                length);
            break;
        default:
            PyErr_SetString(PyExc_NotImplementedError, "Unknown length delimited type");
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
    if (state_get_field_type(state) == vt_fixed32) {
        // TODO(olegs): I'm not sure about the endiannes.
        state->out = PyLong_FromUnsignedLong((uint64_t)(*buf));
    } else {
        state->out = PyLong_FromLong((int64_t)(*buf));
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
            return false;
        default:
            return true;
    }
}

PyObject* parse_message(parse_state* state, char* bytes, size_t len) {
    size_t i = 0;
    size_t j = 0;
    PyObject* dict = PyDict_New();
    
    state->in = cons(strdup(bytes), tstr, nil);

    while (i < len) {
        printf("parsing new field: %zu < %zu\n", i, len);
        j = parse(state);
        if (j == 0) {
            // TODO(olegs): We finished earlier than expected, need to
            // handle this differently.
            break;
        }
        i += j;
        PyDict_SetItem(dict, PyLong_FromLong((long)state->field), state->out);
    }
    print_obj("Parsed message fields: %s\n", dict);
    // PyErr_Print();
    PyObject* factory = PyDict_GetItem(state->factories, state->pytype);
    PyObject* ctor = PyTuple_GetItem(factory, 0);
    print_obj("ctor: %s\n", ctor);
    print_obj("state->pytype: %s\n", state->pytype);
    print_obj("state->factories: %s\n", state->factories);
    print_obj("state->description: %s\n", state->description);
    PyObject* result = PyObject_CallFunction(
        ctor,
        "OOOO",
        state->pytype,
        state->factories,
        state->description,
        dict);
    PyErr_Print();
    state->out = result;
    Py_INCREF(state->out);
    del(state->in);
    return result;
}

PyObject* parse_repeated(parse_state* state, char* bytes, size_t len) {
    size_t i = 0;
    PyObject* result;
    vt_type_t rtype = state_get_field_type(state);

    if (is_scalar(rtype)) {
        parse_handler ph;

        result = PyList_New(0);

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
            i += ph(state);
            PyObject_CallMethod(result, "append", "O", state->out);
        }
    } else {
        // TODO(olegs): Parse custom types
        result = Py_None;
    }

    return result;
}

size_t backtrack(parse_state* state) { return 0; }

size_t select_handler(parse_state* state, parse_handler* handler) {
    if (state_get_available(state) == 0) {
        *handler = backtrack;
        return 0;
    }
    int64_t value[2] = { 0, 0 };
    size_t parsed = parse_varint_impl(state, value);
    size_t wire_type = (size_t)(value[0] & 7);

    state->field = (size_t)(value[0] >> 3);
    printf("varint: %"PRId64"_%"PRId64"\n", value[0], value[1]);
    printf("wire_type: %zu, field: %zu\n", wire_type, state->field);

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
    parse_handler handler = alloca(0);
    size_t parsed = select_handler(state, &handler);
    printf("left to parse: %s\n", str(state->in));
    return parsed + (*handler)(state);
}
