#include <Python.h>

#include <stdio.h>

#include "pyhelpers.h"
#include "list.h"

void print_obj(const char* template, PyObject* obj) {
    PyObject* unicode = PyObject_Str(obj);
    if (!unicode) {
        printf("cannot print object at %p\n", obj);
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

PyObject* list_to_pylist(list elts) {
    Py_ssize_t size = (Py_ssize_t)len(elts);
    PyObject* result = PyList_New(size);
    byte* val;
    size_t vlen;

    size = 0;
    while (!null(elts)) {
        switch (elts->t) {
            case tstr:
                val = (byte*)car(elts);
                vlen = str_size(val);
                PyList_SetItem(result, size, str_to_pystr((char*)(val + 2), vlen));
                break;
            case tint:
                PyList_SetItem(result, size, int_to_pyint((int*)car(elts)));
                break;
            case tlist:
                PyList_SetItem(result, size, list_to_pylist((list)car(elts)));
                break;
            default:
                PyList_SetItem(result, size, Py_None);
        }
        size++;
        elts = cdr(elts);
    }
    return result;
}

list pylist_to_list(PyObject* obj) {
    Py_ssize_t len = PyList_Size(obj);
    Py_ssize_t i = 0;
    list result = nil;
    char* val;
    PyObject* item;
    Py_ssize_t buf_len;

    while (i < len) {
        item = PyList_GetItem(obj, i);
        PyBytes_AsStringAndSize(item, &val, &buf_len);
        result = cons_str(val, (size_t)buf_len, result);
        i++;
    }

    return result;
}
