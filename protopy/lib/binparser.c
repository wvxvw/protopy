#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <Python.h>
#include <alloca.h>
#include "binparser.h"


int64_t state_get_available(parse_state* state) {
    // TODO(olegs): This will become more sophisticated once state can
    // buffer unparsed input.
    return state->available;
}

const char* state_read(parse_state* state, size_t n) {
    // TODO(olegs): This need to account for previously unparsed
    // buffers.
    const char* begin = &state->in[0];
    state->in += n;
    return begin;
}

vt_type_t state_get_value_type(parse_state* state) {
    // TODO(olegs): This should return the type of the value being red
    // based on the description for the object being parsed.
    return vt_int32;
}

size_t parse_varint_impl(parse_state* state, int64_t value[2]) {
    char current;
    size_t read = 0;
    size_t index = 0;

    while (state_get_available(state) > 0 && read < 16) {
        current = state_read(state, 1)[0];
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
    vt_type_t vt = state_get_value_type(state);
    bool sign = false;
    size_t parsed;
    
    if (is_signed_vt(vt)) {
        parsed = parse_zig_zag(state, value, &sign);
    } else {
        parsed = parse_varint_impl(state, value);
    }
    
    PyObject* low = PyLong_FromLong(value[0]);

    if (value[1] > 0) {
        PyObject* high = PyLong_FromLong(value[1]);
        PyObject* shift = PyLong_FromLong(64L);
        PyObject* high_shifted = PyNumber_Lshift(high, shift);
        state->out = PyNumber_Or(low, high_shifted);
    } else {
        state->out = low;
    }
    if (sign) {
        state->out = PyNumber_Negative(state->out);
    }
    PyObject_Print(state->out, stdout, 0);
    return parsed;
}

size_t parse_fixed_64(parse_state* state) {
    return 0;
}

size_t parse_length_delimited(parse_state* state) {
    int64_t value[2] = { 0, 0 };
    size_t parsed = parse_varint_impl(state, value);
    // No reason to care for high bits, we aren't expecting strings of
    // that length anyways.
    int64_t length = value[0];
    PyObject* str;
    const char* bytes;

    while (state_get_available(state) > 0 && length > 0) {
        if (state_get_available(state) > length) {
            bytes = state_read(state, (size_t)length);
            str = PyBytes_FromStringAndSize(bytes, (Py_ssize_t)length);
            length = 0;
            state->out = str;
        } else {
            // We don't have enough bytes available to read the whole thing
        }
    }
    return parsed;
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
    return 0;
}

size_t backtrack(parse_state* state) { return 0; }

size_t select_handler(parse_state* state, parse_handler* handler) {
    if (state_get_available(state) == 0) {
        *handler = backtrack;
        return 0;
    }
    int64_t value[2] = { 0, 0 };
    size_t parsed = parse_varint_impl(state, value);
    size_t wire_type = (size_t)(value[0] & 5);

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
    return parsed + (*handler)(state);
}
