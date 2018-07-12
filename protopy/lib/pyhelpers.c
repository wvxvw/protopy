#include <Python.h>
#include <apr_general.h>
#include <apr_tables.h>
#include <apr_strings.h>

#include <stdio.h>

#include "pyhelpers.h"

void print_obj(const char* template, PyObject* obj) {
    PyObject* unicode = PyObject_Str(obj);
    if (!unicode) {
        printf("cannot print object at %p\n", obj);
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        PyErr_Clear();
        return;
    }
    char* bytes = PyUnicode_AsUTF8(unicode);
    printf(template, bytes);
    Py_DECREF(unicode);
}

apr_array_header_t* pylist_to_array(PyObject* list, apr_pool_t* mp) {
    int len = PyList_Size(list);
    int i = 0;
    apr_array_header_t* result = apr_array_make(mp, 0, sizeof(char*));

    while (i < len) {
        PyObject* item = PyList_GetItem(list, (Py_ssize_t)i);
        char* val = PyBytes_AsString(item);
        char* copy = apr_pstrdup(mp, val);
        APR_ARRAY_PUSH(result, char*) = copy;
        i++;
    }
    return result;
}
