#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include <Python.h>
#include <apr_hash.h>

#include "binparser.h"
#include "pyhelpers.h"
#include "descriptors.h"


static builtin_type_t builtin_types[BUILTIN_TYPES] = {
    {(unsigned char*)"\0"       , vt_error},
    {(unsigned char*)"\0"       , vt_error},
    {(unsigned char*)"bool"     , vt_bool},
    {(unsigned char*)"bytes"    , vt_bytes},
    {(unsigned char*)"double"   , vt_double},
    {(unsigned char*)"fixed32"  , vt_fixed32},
    {(unsigned char*)"fixed64"  , vt_fixed64},
    {(unsigned char*)"int32"    , vt_int32},
    {(unsigned char*)"int64"    , vt_int64},
    {(unsigned char*)"sfixed32" , vt_sfixed32},
    {(unsigned char*)"sfixed64" , vt_sfixed64},
    {(unsigned char*)"sint32"   , vt_sing32},
    {(unsigned char*)"sint64"   , vt_sing64},
    {(unsigned char*)"string"   , vt_string},
    {(unsigned char*)"uint32"   , vt_uint32},
    {(unsigned char*)"uint64"   , vt_uint64},
};

void free_state(PyObject* capsule) {
    if (capsule == Py_None) {
        return;
    }
    parse_state_t* state = (parse_state_t*)PyCapsule_GetPointer(capsule, NULL);
    free(state);
}

PyObject* state_ready(PyObject* self, PyObject* args) {
    PyObject* capsule;
    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    parse_state_t* state = (parse_state_t*)PyCapsule_GetPointer(capsule, NULL);
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
    parse_state_t* state = (parse_state_t*)PyCapsule_GetPointer(capsule, NULL);
    if (!state) {
        return NULL;
    }
    return state->out;
}

PyObject* make_state(PyObject* self, PyObject* args) {
    apr_hash_t* factories;
    char* pytype;
    apr_pool_t* mp;
    PyObject* factories_capsule;
    PyObject* mp_capsule;

    if (!PyArg_ParseTuple(
            args,
            "yOO",
            &pytype,
            &factories_capsule,
            &mp_capsule)) {
        return NULL;
    }

    factories = (apr_hash_t*)PyCapsule_GetPointer(factories_capsule, NULL);
    if (!factories) {
        PyErr_Format(PyExc_ValueError, "Factories weren't initialized");
        return NULL;
    }

    mp = (apr_pool_t*)PyCapsule_GetPointer(mp_capsule, NULL);
    if (!mp) {
        PyErr_Format(PyExc_ValueError, "APR memory wasn't initialized");
        return NULL;
    }

    parse_state_t* state = malloc(sizeof(parse_state_t));
    state->pos = 0;
    state->out = Py_None;
    state->pytype = cstr_bytes(pytype);
    state->factories = factories;
    state->factory = NULL;
    state->mp = mp;

    return PyCapsule_New(state, NULL, free_state);
}

int64_t state_get_available(parse_state_t* const state) {
    return state->len - state->pos;
}

size_t state_read(parse_state_t* const state, unsigned char** buf, size_t n) {
    *buf = &state->in[state->pos];
    if (state->len >= state->pos + (int64_t)n) {
        state->pos += (int64_t)n;
        return n;
    }
    state->pos = state->len;
    return state->len - state->pos;
}

size_t parse_varint_impl(parse_state_t* const state, uint64_t value[2]) {
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

size_t parse_zig_zag(parse_state_t* const state, uint64_t value[2], bool* is_neg) {
    size_t parsed = parse_varint_impl(state, value);
    *is_neg = (value[0] & 1) == 1;
    uint64_t high = value[1];
    uint64_t low = value[0];
    low = ((high & 1) << 63) | ((low + *is_neg) >> 1);
    high >>= 1;
    value[0] = low;
    value[1] = high;
    return parsed;
}

bool is_signed_vt(vt_type_t vt) {
    return vt == vt_sing32 || vt == vt_sing64;
}

PyObject* tuple_from_dict(factory_t* factory, apr_hash_t* values) {
    apr_hash_index_t* hi;
    void* val;
    const void* key;
    apr_pool_t* mp = apr_hash_pool_get(values);
    size_t max = 0;
    PyObject* result;

    for (hi = apr_hash_first(mp, factory->mapping); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, &key, NULL, &val);
        field_info_t* info = (field_info_t*)val;
        if (max <= info->n) {
            max = info->n + 1;
        }
    }
    if (max == 0) {
        result = PyObject_Call(factory->ctor, PyTuple_New(0), NULL);
        if (result) {
            Py_INCREF(result);
            return result;
        }
    }

    PyObject* args = PyTuple_New(max);

    while (max > 0) {
        max--;
        Py_INCREF(Py_None);
        PyTuple_SetItem(args, max, Py_None);
    }

    for (hi = apr_hash_first(mp, values); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, &key, NULL, &val);
        field_info_t* info = apr_hash_get(factory->mapping, key, sizeof(size_t));
        PyTuple_SetItem(args, info->n, val);
    }

    result = PyObject_Call(factory->ctor, args, NULL);
    if (result) {
        Py_INCREF(result);
    }
    return result;
}

size_t parse_varint(parse_state_t* const state, field_info_t* const info) {
    uint64_t value[2] = { 0, 0 };
    bool sign = false;
    size_t parsed;

    if (is_signed_vt(info->vt_type)) {
        parsed = parse_zig_zag(state, value, &sign);
    } else {
        parsed = parse_varint_impl(state, value);
    }
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
    if (info->vt_type == vt_enum) {
        factory_t* factory = apr_hash_get(
            state->factories,
            bytes_cstr(info->pytype),
            APR_HASH_KEY_STRING);
        state->out = PyObject_CallFunctionObjArgs(
            factory->ctor,
            state->out,
            NULL);
    }
    Py_INCREF(state->out);
    return parsed;
}

size_t parse_fixed_64(parse_state_t* const state, field_info_t* const info) {
#define FIXED_LENGTH 8
    unsigned char* buf = NULL;
    size_t read = state_read(state, &buf, FIXED_LENGTH);

    if (read < FIXED_LENGTH) {
        PyErr_Format(
            PyExc_ValueError,
            "Not enough bytes to parse fixed64: %zu", read);
        return read;
    }
    unsigned long long val = 0;
    // ntohl only takes up to 32 bits
    while (read > 0) {
        read--;
        val <<= 8;
        val |= (unsigned long long)buf[read];
    }

    switch (info->vt_type) {
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

void init_substate(
    parse_state_t* substate,
    parse_state_t* parent,
    unsigned char* bytes,
    uint64_t length,
    field_info_t* info) {

    substate->pos = 0;
    substate->in = bytes;
    substate->len = length;
    substate->factories = parent->factories;
    substate->pytype = info->pytype;
    substate->mp = parent->mp;
}

size_t parse_length_delimited(parse_state_t* const state, field_info_t* const info) {
    uint64_t value[2] = { 0, 0 };
    size_t parsed = parse_varint_impl(state, value);

    if (PyErr_Occurred()) {
        return parsed;
    }
    // No reason to care for high bits, we aren't expecting strings of
    // that length anyways.
    uint64_t length = value[0];
    unsigned char* bytes = NULL;
    size_t read = state_read(state, &bytes, length);
    parse_state_t substate;

    if (read != length) {
        PyErr_Format(
            PyExc_ValueError,
            "Not enough bytes to parse length-delimited: %zu", read);
        return read;
    }

    switch (info->vt_type) {
        case vt_string:
            state->out = PyUnicode_FromStringAndSize((char*)bytes, (Py_ssize_t)length);
            break;
        case vt_bytes:
            state->out = PyBytes_FromStringAndSize((char*)bytes, (Py_ssize_t)length);
            break;
        case vt_message:
            init_substate(&substate, state, bytes, length, info);
            state->out = parse_message(&substate);
            break;
        case vt_repeated:
            init_substate(&substate, state, bytes, length, info);
            state->out = parse_repeated(&substate, info);
            break;
        case vt_map:
            init_substate(&substate, state, bytes, length, info);
            state->out = parse_map(&substate, info);
            break;
        default:
            if (!PyErr_Occurred()) {
                PyErr_SetString(
                    PyExc_NotImplementedError,
                    "Unknown length delimited type");
            }
    }
    if (state->out) {
        Py_INCREF(state->out);
    }
    return parsed + read;
}

size_t parse_start_group(parse_state_t* const state, field_info_t* const info) {
    PyErr_SetString(PyExc_NotImplementedError, "Proto v2 not supported");
    return 0;
}

size_t parse_end_group(parse_state_t* const state, field_info_t* const info) {
    PyErr_SetString(PyExc_NotImplementedError, "Proto v2 not supported");
    return 0;
}

size_t parse_fixed_32(parse_state_t* const state, field_info_t* const info) {
#define FIXED_LENGTH 4
    unsigned char* buf = NULL;
    size_t read = state_read(state, &buf, FIXED_LENGTH);

    if (read < FIXED_LENGTH) {
        PyErr_Format(
            PyExc_ValueError,
            "Not enough bytes to parse fixed32: %zu", read);
        return read;
    }
    
    unsigned long val = 0;
    while (read > 0) {
        read--;
        val <<= 8;
        val |= (unsigned long)buf[read];
    }
    if (info->vt_type == vt_fixed32) {
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

vt_type_t vt_builtin(byte* type) {
    size_t len = BUILTIN_TYPES;
    size_t i = len >> 1;
    size_t tlen = str_size(type) + 2;
    size_t pos;
    size_t step = i;
    char a, b;
    builtin_type_t* bt;

    while (step > 0) {
        bt = &builtin_types[i];
        pos = 2;
        do {
            a = type[pos];
            b = bt->name[pos - 2];
            pos++;
        } while (a == b && pos < tlen);
        if (pos == tlen) {
            return bt->value;
        }
        step >>= 1;
        if (a > b) {
            i += step;
        } else {
            i -= step;
        }
        if (i >= BUILTIN_TYPES) {
            break;
        }
    }
    return vt_default;
}

void resolve_type(parse_state_t* const state, byte* pytype, vt_type_t* vttype) {
    *vttype = vt_builtin(pytype);
    if (*vttype == vt_default) {
        factory_t* f = apr_hash_get(
            state->factories,
            bytes_cstr(pytype),
            APR_HASH_KEY_STRING);
        if (f) {
            *vttype = f->vt_type;
        }
    }
    if (*vttype == vt_default) {
        *vttype = vt_error;
        PyErr_Format(
            PyExc_TypeError,
            "No definition for type: %s", bytes_cstr(pytype));
    }
}

PyObject* parse_message(parse_state_t* const state) {
    int64_t i;
    apr_hash_t* fields = apr_hash_make(state->mp);
    bool read_key = true;
    uint64_t value[2] = { 0, 0 };
    size_t wiretype = 0;
    field_info_t* info = NULL;

    state->factory = apr_hash_get(
        state->factories,
        bytes_cstr(state->pytype),
        APR_HASH_KEY_STRING);

    while (state->pos < state->len) {
        i = state->pos;
        if (read_key) {
            value[0] = value[1] = 0;
            parse_varint_impl(state, value);
            wiretype = (size_t)(value[0] & 7);
            state->field = (size_t)(value[0] >> 3);
            info = apr_hash_get(state->factory->mapping, &state->field, sizeof(size_t));
            if (!info) {
                PyErr_Format(
                    PyExc_TypeError,
                    "Don't know how to parse field %zu of %s",
                    state->field,
                    state->pytype);
                break;
            }
            if (info->vt_type == vt_default) {
                resolve_type(state, info->pytype, &info->vt_type);
            } else if (info->vt_type == vt_repeated) {
                resolve_type(state, info->pytype, &info->extra_type_info.elt);
            } else if (info->vt_type == vt_map) {
                resolve_type(
                    state,
                    info->extra_type_info.pair.pyval,
                    &info->extra_type_info.pair.val);
            }
            if (info->vt_type == vt_error) {
                break;
            }
        } else {
            switch (wiretype) {
                case 0:
                    parse_varint(state, info);
                    break;
                case 1:
                    parse_fixed_64(state, info);
                    break;
                case 2:
                    parse_length_delimited(state, info);
                    break;
                case 3:
                    parse_start_group(state, info);
                    break;
                case 4:
                    parse_end_group(state, info);
                    break;
                case 5:
                    parse_fixed_32(state, info);
                    break;
                case 6:
                case 7:
                    PyErr_SetString(
                        PyExc_NotImplementedError,
                        "Invalid encoding, wire_type 6 and 7 are not defined");
            }
        }
        if (i == state->pos || PyErr_Occurred()) {
            break;
        }
        if (!read_key) {
            if (wiretype != 2) {
                size_t* key = apr_palloc(state->mp, sizeof(size_t));
                *key = state->field;
                apr_hash_set(fields, key, sizeof(size_t), state->out);
            } else {
                PyObject* val = apr_hash_get(fields, &state->field, sizeof(size_t));
                if (val && PyList_CheckExact(val)) {
                    PyList_Append(val, PyList_GetItem(state->out, 0));
                    Py_DECREF(state->out);
                } else if (val && PyDict_CheckExact(val)) {
                    PyDict_Update(val, state->out);
                    Py_DECREF(state->out);
                } else {
                    size_t* key = apr_palloc(state->mp, sizeof(size_t));
                    *key = state->field;
                    apr_hash_set(fields, key, sizeof(size_t), state->out);
                }
            }
        }
        read_key = !read_key;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }
    state->out = tuple_from_dict(state->factory, fields);
    return state->out;
}

PyObject* parse_map(parse_state_t* const state, field_info_t* info) {
    PyObject* result = PyDict_New();
    uint64_t val[2] = { 0, 0 };
    
    field_info_t key_info;
    field_info_t val_info;
    field_info_t* current_info;

    PyObject* key = Py_None;
    PyObject* value = Py_None;
    size_t i = 0;
    
    key_info.vt_type = info->extra_type_info.pair.key;
    val_info.vt_type = info->extra_type_info.pair.val;
    val_info.pytype = info->extra_type_info.pair.pyval;

    while (i < 2) {
        val[0] = val[1] = 0;
        parse_varint_impl(state, val);
        size_t wiretype = (size_t)(val[0] & 7);
        state->field = (size_t)(val[0] >> 3);

        if (state->field == 1) {
            current_info = &key_info;
        } else {
            current_info = &val_info;
        }

        switch (wiretype) {
            case 0:
                parse_varint(state, current_info);
                break;
            case 1:
                parse_fixed_64(state, current_info);
                break;
            case 2:
                parse_length_delimited(state, current_info);
                break;
            case 3:
                parse_start_group(state, current_info);
                break;
            case 4:
                parse_end_group(state, current_info);
                break;
            case 5:
                parse_fixed_32(state, current_info);
                break;
            case 6:
            case 7:
                PyErr_SetString(
                    PyExc_NotImplementedError,
                    "Invalid encoding, wire_type 6 and 7 are not defined");
        }

        if (PyErr_Occurred()) {
            Py_DECREF(result);
            return NULL;
        }

        if (state->field == 1) {
            key = state->out;
        } else {
            value = state->out;
        }
        i++;
    }

    PyDict_SetItem(result, key, value);
    return result;
}

PyObject* parse_repeated(parse_state_t* const state, field_info_t* const info) {
    int64_t i = 0;
    size_t j = 0;
    PyObject* result = PyList_New(0);
    PyObject* subresult;
    field_info_t rinfo;
    rinfo.n = info->n;
    rinfo.vt_type = info->extra_type_info.elt;
    rinfo.pytype = info->pytype;

    if (is_scalar(rinfo.vt_type)) {
        parse_handler ph;

        switch (wiretype_of(rinfo.vt_type)) {
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
            j = ph(state, &rinfo);
            if (j == 0) {
                break;
            }
            i += (int64_t)j;
            PyList_Append(result, state->out);
            Py_DECREF(state->out);
        }
    } else if (rinfo.vt_type == vt_string) {
        subresult = PyUnicode_FromStringAndSize((char*)state->in, (Py_ssize_t)state->len);
        PyList_Append(result, subresult);
        Py_DECREF(subresult);
    } else if (rinfo.vt_type == vt_bytes) {
        subresult = PyBytes_FromStringAndSize((char*)state->in, (Py_ssize_t)state->len);
        PyList_Append(result, subresult);
        Py_DECREF(subresult);
    } else {
        state->pytype = info->pytype;

        subresult = parse_message(state);
        if (!subresult) {
            return NULL;
        }
        PyList_Append(result, subresult);
        Py_DECREF(subresult);
    }
    return result;
}
