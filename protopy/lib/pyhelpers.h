#ifndef PYHELPERS_H_
#define PYHELPERS_H_

#include <Python.h>
#include <apr_general.h>
#include <apr_tables.h>

#ifdef __cplusplus
extern "C" {
#endif

void print_obj(const char*, PyObject*);

apr_array_header_t* pylist_to_array(PyObject*, apr_pool_t*);

#ifdef __cplusplus
}
#endif

#endif // PYHELPERS_H_
