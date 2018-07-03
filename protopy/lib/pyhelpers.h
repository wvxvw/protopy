#ifndef PYHELPERS_H_
#define PYHELPERS_H_

#include <Python.h>
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

void print_obj(const char*, PyObject*);

PyObject* str_to_pystr(char*, size_t);

PyObject* int_to_pyint(int*);

PyObject* list_to_pylist(list_t*);

list_t* pylist_to_list(PyObject*, apr_pool_t*);

#ifdef __cplusplus
}
#endif

#endif // PYHELPERS_H_
