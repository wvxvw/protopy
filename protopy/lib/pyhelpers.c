#include <Python.h>
#include <alloca.h>
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

PyObject* py_string(char* val) {
    printf("py_string: %s\n", val);
    PyObject* result = PyBytes_FromStringAndSize(val, strlen(val));
    Py_INCREF(result);
    printf("py_string returns: %s\n", PyBytes_AsString(result));
    return result;
}

PyObject* py_string_append(PyObject* to, char* part) {
    printf("py_string_append: %s\n", part);
    PyObject* add = PyBytes_FromStringAndSize(part, strlen(part));
    PyBytes_Concat(&to, add);
    return to;
}

PyObject* py_string_concat(PyObject* to, PyObject* part) {
    printf("py_string_concat\n");
    PyBytes_Concat(&to, part);
    return to;
}

PyObject* py_oneof(char* val) {
    return Py_None;
}

PyObject* py_enum(char* name, PyObject* fields) {
    printf("py_enum: %s\n", name);
    PyObject* enum_module = PyImport_ImportModule("enum");
    PyObject* enum_class = PyObject_GetAttrString(enum_module, "Enum");
    PyObject* subclass_name = Py_BuildValue("#s", name, strlen(name));
    PyObject* args = Py_BuildValue("(OO)", subclass_name, fields);
    return PyObject_CallObject(enum_class, args);
}

PyObject* py_list(PyObject* obj) {
    print_obj("py_list: %s\n", obj);
    PyObject* result;
    if (obj == NULL) {
        result = PyList_New(0);
        Py_INCREF(result);
        return result;
    }
    result = PyList_New(1);
    PyList_SetItem(result, 0, obj);
    Py_INCREF(result);
    print_obj("py_list returns: %s\n", result);
    return result;
}

PyObject* py_append(PyObject* list, PyObject* obj) {
    printf("py_append\n");
    if (obj != NULL) {
        PyList_Append(list, obj);
    }
    return list;
}

PyObject* py_named_tuple(char* name, PyObject* fields) {
    printf("py_named_tuple: %s\n", name);
    print_obj("py_named_tuple fileds: %s\n", fields);
    Py_ssize_t numfields = PyList_Size(fields);
    if (PyErr_Occurred()) {
        printf("error after list size\n");
    }
    Py_ssize_t i, s;
    PyObject* item;
    char* item_chars;
    PyStructSequence_Field* sfields =
            (PyStructSequence_Field*)alloca(sizeof(PyStructSequence_Field) * numfields);

    for (i = 0; i < numfields; i++) {
        item = PyList_GetItem(fields, i);
        print_obj("item: %s\n", item);
        s = PyBytes_Size(item);
        item_chars = PyBytes_AsString(item);
        printf("item_chars[%d] = %d\n", (int)s, item_chars[s]);
        printf("item_chars: %d, %s\n", (int)s, item_chars);
        sfields[i].name = item_chars;
        sfields[i].doc = "dummy doc string";
    }
    if (PyErr_Occurred()) {
        printf("error after initializing fields\n");
    }
    
    // NOTE(olegs): My reading of structseq.c is that this is a
    // temporary object, not used after PyStructSequence_NewType
    // returns.
    PyStructSequence_Desc desc = {
        name,
        "dummy doc string",
        sfields,
        (int)numfields,
    };

    PyObject* result = (PyObject*)PyStructSequence_NewType(&desc);
    
    if (PyErr_Occurred()) {
        printf("error after new type: %p\n", result);
        print_obj("error after new type: %s\n", result);
    }
    if (result != NULL) {
        Py_INCREF(result);
    }
    return result;
}

PyObject* py_service(char* name, PyObject* methods) {
    printf("py_service: %s\n", name);
    return Py_None;
}

PyObject* py_method(char* name) {
    printf("py_method: %s\n", name);
    return Py_None;
}

PyObject* str_to_pystr(char* val) {
    return Py_BuildValue("y", val);
}

PyObject* int_to_pyint(int* val) {
    return Py_BuildValue("i", *val);
}

PyObject* list_to_pylist(list elts) {
    Py_ssize_t size = (Py_ssize_t)len(elts);
    PyObject* result = PyList_New(size);

    size = 0;
    while (!null(elts)) {
        switch (elts->t) {
            case tstr:
                PyList_SetItem(result, size, str_to_pystr((char*)car(elts)));
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
    Py_INCREF(result);
    return result;
}

list pylist_to_list(PyObject* obj) {
    Py_ssize_t len = PyList_Size(obj);
    Py_ssize_t i = 0;
    list result = nil;
    char* val;
    PyObject* item;

    while (i < len) {
        item = PyList_GetItem(obj, i);
        PyArg_Parse(item, "y", &val);
        result = cons(strdup(val), tstr, result);
        i++;
    }

    return result;
}
