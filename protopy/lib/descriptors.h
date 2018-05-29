#ifndef DESCRIPTIORS_H_
#define DESCRIPTIORS_H_

#include <apr_general.h>
#include <Python.h>

#include "list.h"
#include "binparser.h"

typedef struct _field_info {
    const size_t n;
    const byte* pytype;
} field_info_t;

typedef struct _factory {
    const vt_type_t vt_type;
    const field_info_t field;
    apr_hash_t* mapping;
    PyObject* ctor;
} factory_t;

void extract_type_name(const byte*, byte**, byte**, apr_pool_t*);

void
enum_desc(const byte*, const list, apr_hash_t* const, apr_hash_t* const, apr_pool_t* const);

void
message_desc(const byte*, const list, apr_hash_t* const, apr_hash_t* const, apr_pool_t* const);

apr_hash_t*
create_descriptors(const apr_hash_t* const, apr_pool_t* const);

#endif // DESCRIPTIORS_H_
