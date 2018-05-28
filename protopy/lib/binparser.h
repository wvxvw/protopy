#ifndef BINPARSER_H_
#define BINPARSER_H_

#include <Python.h>

#include "list.h"

typedef enum vt_type_t {
    // varint
    vt_int32  = 0,
    vt_int64  = 1,
    vt_uint32 = 2,
    vt_uint64 = 3,
    vt_sing32 = 4,
    vt_sing64 = 5,
    vt_bool   = 6,
    vt_enum   = 7,
    // 64 bit
    vt_fixed64  = 8,
    vt_sfixed64 = 9,
    vt_double   = 10,
    // length-delimited
    vt_string   = 11,
    vt_bytes    = 12,
    vt_message  = 13,
    vt_repeated = 14,
    // 32 bit
    vt_fixed32  = 15,
    vt_sfixed32 = 16,
    // not exposed to user
    vt_error   = 17,
    vt_default = 18,
    vt_map = 19
} vt_type_t;

typedef enum wiretype_t {
    wt_varint = 0,
    wt_fixed32 = 1,
    wt_fixed64 = 2,
    wt_string = 3
} wiretype_t;

wiretype_t wiretype_of(vt_type_t);

bool is_scalar(vt_type_t);


typedef struct {
    int64_t pos;
    int64_t len;
    size_t field;
    unsigned char* in;
    bool is_field;
    PyObject* out;
    PyObject* factories;
    PyObject* pytype;
    PyObject* builtin_types;
} parse_state;

typedef size_t (*parse_handler)(parse_state* const);

int64_t state_get_available(parse_state* const);

vt_type_t state_get_repeated_type(parse_state* const);

vt_type_t state_get_field_type(parse_state* const);

size_t state_read(parse_state* const, unsigned char**, size_t);

size_t parse(parse_state* const);

size_t parse_varint(parse_state* const);

size_t parse_fixed_64(parse_state* const);

size_t parse_length_delimited(parse_state* const);

size_t parse_start_group(parse_state* const);

size_t parse_end_group(parse_state* const);

size_t parse_fixed_32(parse_state* const);

PyObject* parse_message(parse_state* const);

PyObject* parse_repeated(parse_state* const);

PyObject* parse_map(parse_state* const);

size_t select_handler(parse_state* const, parse_handler*);

PyObject* make_state(PyObject*, PyObject*);

PyObject* state_ready(PyObject*, PyObject*);

PyObject* state_result(PyObject*, PyObject*);

PyObject* state_set_factory(PyObject*, PyObject*);

#endif // BINPARSER_H_
