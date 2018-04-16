#ifndef BINPARSER_H_
#define BINPARSER_H_

#include <Python.h>

typedef struct {
    int64_t pos;
    const char* in;
    int64_t available;
    PyObject* out;
    size_t field;
} parse_state;

typedef size_t (*parse_handler)(parse_state*);

int64_t state_get_available(parse_state*);

const char* state_read(parse_state*, size_t);

size_t parse(parse_state*);

size_t parse_varint(parse_state*);

size_t parse_fixed_64(parse_state*);

size_t parse_length_delimited(parse_state*);

size_t parse_start_group(parse_state*);

size_t parse_end_group(parse_state*);

size_t parse_fixed_32(parse_state*);

#endif // BINPARSER_H_
