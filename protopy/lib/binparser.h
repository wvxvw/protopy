#ifndef BINPARSER_H_
#define BINPARSER_H_

#include <Python.h>
#include <apr_hash.h>

#include "list.h"
#include "descriptors.h"

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
    apr_hash_t* factories;
    apr_pool_t* mp;
    factory_t* factory;
    byte* pytype;
    PyObject* out;
    PyObject* builtin_types;
} parse_state_t;

typedef size_t (*parse_handler)(parse_state_t* const, field_info_t* const);

int64_t state_get_available(parse_state_t* const);

vt_type_t state_get_repeated_type(parse_state_t* const);

size_t state_read(parse_state_t* const, unsigned char**, size_t);

size_t parse_varint(parse_state_t* const, field_info_t* const);

size_t parse_fixed_64(parse_state_t* const, field_info_t* const);

size_t parse_length_delimited(parse_state_t* const, field_info_t* const);

size_t parse_start_group(parse_state_t* const, field_info_t* const);

size_t parse_end_group(parse_state_t* const, field_info_t* const);

size_t parse_fixed_32(parse_state_t* const, field_info_t* const);

PyObject* parse_message(parse_state_t* const);

PyObject* parse_repeated(parse_state_t* const, field_info_t* const);

PyObject* parse_map(parse_state_t* const);

PyObject* make_state(PyObject*, PyObject*);

PyObject* state_ready(PyObject*, PyObject*);

PyObject* state_result(PyObject*, PyObject*);

void resolve_type(parse_state_t* const, field_info_t*);

#endif // BINPARSER_H_
