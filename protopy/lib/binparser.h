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
    vt_sfixed32 = 16
} vt_type_t;


typedef struct {
    int64_t pos;
    size_t field;
    list in;
    PyObject* out;
    PyObject* description;  // dict
    PyObject* current_description;  // function(x: bytes) -> object
} parse_state;

typedef size_t (*parse_handler)(parse_state*);

int64_t state_get_available(parse_state*);

vt_type_t state_get_value_type(parse_state*);

size_t state_read(parse_state*, char*, size_t);

size_t parse(parse_state*);

size_t parse_varint(parse_state*);

size_t parse_fixed_64(parse_state*);

size_t parse_length_delimited(parse_state*);

size_t parse_start_group(parse_state*);

size_t parse_end_group(parse_state*);

size_t parse_fixed_32(parse_state*);

#endif // BINPARSER_H_
