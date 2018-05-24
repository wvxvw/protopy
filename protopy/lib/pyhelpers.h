#ifndef PYHELPERS_H_
#define PYHELPERS_H_

#include <Python.h>
#include "list.h"


void print_obj(const char*, PyObject*);

PyObject* str_to_pystr(char*, size_t);

PyObject* int_to_pyint(int*);

PyObject* list_to_pylist(list);

list pylist_to_list(PyObject*);

#endif // PYHELPERS_H_
