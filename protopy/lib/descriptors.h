#ifndef DESCRIPTIORS_H_
#define DESCRIPTIORS_H_

#include <apr_general.h>
#include <Python.h>

#include "list.h"
#include "binparser.h"

typedef struct _field_info {
    size_t n;
    vt_type_t vt_type;
    byte* pytype;
} field_info_t;

typedef struct _factory {
    vt_type_t vt_type;
    apr_hash_t* mapping;
    PyObject* ctor;
} factory_t;

void extract_type_name(const byte*, byte**, byte**);

void
enum_desc(const byte*, const list, apr_hash_t* const, PyObject*, apr_pool_t* const);

void
message_desc(const byte*, const list, apr_hash_t* const, PyObject*, apr_pool_t* const);

apr_hash_t*
create_descriptors(apr_hash_t* const, PyObject*, PyObject*, apr_pool_t* const);

#endif // DESCRIPTIORS_H_
