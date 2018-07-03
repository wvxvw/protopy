#include <Python.h>

#include <stdio.h>

#include "pyhelpers.h"
#include "list.h"

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

PyObject* str_to_pystr(char* val, size_t len) {
    return PyBytes_FromStringAndSize(val, (Py_ssize_t)len);
}

PyObject* int_to_pyint(int* val) {
    return PyLong_FromLong((long)(*val));
}

PyObject* list_to_pylist(list_t* elts) {
    Py_ssize_t size = (Py_ssize_t)len(elts);
    PyObject* result = PyList_New(size);
    PyObject* elt;
    byte* val;
    size_t vlen;

    size = 0;
    while (!null(elts)) {
        switch (elts->t) {
            case tstr:
                val = (byte*)car(elts);
                vlen = str_size(val);
                elt = str_to_pystr((char*)(val + 2), vlen);
                break;
            case tint:
                elt = int_to_pyint((int*)car(elts));
                break;
            case tlist:
                elt = list_to_pylist((list_t*)car(elts));
                break;
            default:
                elt = Py_None;
        }
        Py_INCREF(elt);
        PyList_SetItem(result, size, elt);
        size++;
        elts = cdr(elts);
    }
    return result;
}

list_t* pylist_to_list(PyObject* obj, apr_pool_t* mp) {
    Py_ssize_t len = PyList_Size(obj);
    Py_ssize_t i = 0;
    list_t* result = nil;
    char* val;
    PyObject* item;
    Py_ssize_t buf_len;

    while (i < len) {
        item = PyList_GetItem(obj, i);
        PyBytes_AsStringAndSize(item, &val, &buf_len);
        result = cons_str(val, (size_t)buf_len, result, mp);
        i++;
    }

    return result;
}
