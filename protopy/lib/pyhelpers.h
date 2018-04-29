#ifndef PYHELPERS_H_
#define PYHELPERS_H_

#include <Python.h>
#include "list.h"

PyObject* py_string(char*);

PyObject* py_string_append(PyObject*, char*);

PyObject* py_string_concat(PyObject*, PyObject*);

PyObject* py_oneof(char*);

PyObject* py_enum(char*, PyObject*);

PyObject* py_list(PyObject*);

PyObject* py_append(PyObject*, PyObject*);

PyObject* py_named_tuple(char*, PyObject*);

PyObject* py_service(char*, PyObject*);

PyObject* py_method(char*);

void print_obj(const char*, PyObject*);

PyObject* str_to_pystr(char*);

PyObject* int_to_pyint(int*);

PyObject* list_to_pylist(list);

list pylist_to_list(PyObject*);

#endif // PYHELPERS_H_
