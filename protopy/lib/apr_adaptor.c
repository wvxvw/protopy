#include <Python.h>
#include <apr_general.h>
#include <apr_hash.h>
#include <apr_tables.h>

#include "helpers.h"
#include "pyhelpers.h"
#include "descriptors.h"
#include "apr_adaptor.h"

bool apr_terminated = false;

void free_apr_hash(PyObject* capsule) { }

PyObject* factory_to_pytype(void* val) {
    factory_t* f = val;
    if (f) {
        return f->ctor;
    }
    return NULL;
}

PyObject* protofile_to_ptytpe(void* val) {
    proto_file_t* pf = val;
    if (!pf) {
        return NULL;
    }
    int i = 0;
    PyObject* result = PyTuple_New(2);
    PyObject* messages = PyList_New(0);
    PyObject* enums = PyList_New(0);

    Py_INCREF(messages);
    Py_INCREF(enums);
    PyTuple_SetItem(result, 0, messages);
    PyTuple_SetItem(result, 1, enums);

    while (i < pf->messages->nelts) {
        proto_message_t* m = APR_ARRAY_IDX(pf->messages, i, proto_message_t*);
        PyList_Append(messages, PyBytes_FromString(mdupstr(m->t)));
        i++;
    }

    i = 0;
    while (i < pf->enums->nelts) {
        proto_enum_t* m = APR_ARRAY_IDX(pf->enums, i, proto_enum_t*);
        PyList_Append(messages, PyBytes_FromString(mdupstr(m->t)));
        i++;
    }
    return result;
}

const apr_to_py_fn_t converters[2] = {
    factory_to_pytype,
    protofile_to_ptytpe,
};

PyObject* apr_hash_replace(PyObject* self, PyObject* args) {
    PyObject* capsule;
    char* key;
    factory_t* val;
    PyObject* pyval;

    if (!PyArg_ParseTuple(args, "OyO", &capsule, &key, &pyval)) {
        return NULL;
    }
    apr_hash_t* ht = (apr_hash_t*)PyCapsule_GetPointer(capsule, NULL);
    if (!ht) {
        PyErr_Format(PyExc_ValueError, "Missing hash table");
        return NULL;
    }
    // TODO(olegs): Maybe decref previous value if it existed?
    val = (factory_t*)apr_hash_get(ht, key, APR_HASH_KEY_STRING);
    if (val) {
        val->ctor = pyval;
        Py_INCREF(val->ctor);
    } else {
        PyErr_Format(PyExc_KeyError, "Types %s is not registered", key);
        return NULL;
    }
    Py_RETURN_NONE;
}

PyObject* apr_hash_iterator(PyObject* self, PyObject* args) {
    PyObject* capsule;

    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    apr_hash_t* ht = (apr_hash_t*)PyCapsule_GetPointer(capsule, NULL);
    if (!ht) {
        PyErr_Format(PyExc_ValueError, "Missing hash table");
        return NULL;
    }
    apr_pool_t* mp = apr_hash_pool_get(ht);
    apr_hash_index_t* hi = apr_hash_first(mp, ht);
    if (hi) {
        return PyCapsule_New(hi, NULL, free_apr_hash);
    }
    Py_RETURN_NONE;
}

PyObject* apr_hash_get_kv(PyObject* self, PyObject* args) {
    PyObject* capsule;
    Py_ssize_t nconverter;

    if (!PyArg_ParseTuple(args, "On", &capsule, &nconverter)) {
        return NULL;
    }
    void* maybe_hi = PyCapsule_GetPointer(capsule, NULL);
    PyObject* result = PyTuple_New(2);

    if (!maybe_hi || maybe_hi == (void*)0xdeadbeef) {
        PyTuple_SetItem(result, 0, Py_None);
        PyTuple_SetItem(result, 1, Py_None);
        return result;
    }
    apr_hash_index_t* hi = (apr_hash_index_t*)maybe_hi;

    const void* key;
    void* val;
    apr_hash_this(hi, &key, NULL, &val);
    PyObject* pykey = PyBytes_FromString(mdupstr(key));
    apr_to_py_fn_t converter = converters[nconverter];
    PyObject* pyval = converter(val);
    if (!pyval) {
        Py_DECREF(result);
        Py_DECREF(pykey);
        if (!PyErr_Occurred()) {
            PyErr_Format(PyExc_ValueError, "Couldn't convert value for key: %A\n", pykey);
        }
        return NULL;
    }
    Py_INCREF(pyval);
    Py_INCREF(pykey);
    Py_INCREF(pyval);
    PyTuple_SetItem(result, 0, pykey);
    PyTuple_SetItem(result, 1, pyval);
    hi = apr_hash_next(hi);
    if (!hi) {
        PyCapsule_SetPointer(capsule, (void*)0xdeadbeef);
    } else {
        PyCapsule_SetPointer(capsule, hi);
    }
    return result;
}

PyObject* make_apr_hash(PyObject* self, PyObject* args) {
    PyObject* capsule;

    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    apr_pool_t* mp = (apr_pool_t*)PyCapsule_GetPointer(capsule, NULL);
    if (!mp) {
        PyErr_Format(PyExc_ValueError, "Missing APR memory pool");
        return NULL;
    }
    apr_hash_t* ht = apr_hash_make(mp);
    return PyCapsule_New(ht, NULL, free_apr_hash);
}

PyObject* apr_update_hash(PyObject* self, PyObject* args) {
    PyObject* from_capsule;
    PyObject* to_capsule;

    if (!PyArg_ParseTuple(args, "OO", &to_capsule, &from_capsule)) {
        return NULL;
    }
    apr_hash_t* from = (apr_hash_t*)PyCapsule_GetPointer(from_capsule, NULL);
    if (!from) {
        PyErr_Format(PyExc_ValueError, "Missing hash table to update from");
        return NULL;
    }
    apr_hash_t* to = (apr_hash_t*)PyCapsule_GetPointer(to_capsule, NULL);
    if (!from) {
        PyErr_Format(PyExc_ValueError, "Missing hash table to update");
        return NULL;
    }
    apr_pool_t* mp = apr_hash_pool_get(to);
    apr_hash_t* result = apr_hash_overlay(mp, from, to);
    PyCapsule_SetPointer(to_capsule, result);
    Py_RETURN_NONE;
}

PyObject* apr_hash_find(PyObject* self, PyObject* args) {
    PyObject* capsule;
    char* key;
    Py_ssize_t nconverter;

    // TODO(olegs): Do we need to free key?
    if (!PyArg_ParseTuple(args, "Oyn", &capsule, &key, &nconverter)) {
        return NULL;
    }
    apr_hash_t* ht = (apr_hash_t*)PyCapsule_GetPointer(capsule, NULL);
    if (!ht) {
        PyErr_Format(PyExc_ValueError, "Missing hash table");
        return NULL;
    }
    void* result = apr_hash_get(ht, key, APR_HASH_KEY_STRING);
    if (result) {
        apr_to_py_fn_t converter = converters[nconverter];
        PyObject* pyresult = converter(result);
        if (!pyresult) {
            return NULL;
        }
        Py_INCREF(pyresult);
        return pyresult;
    }
    Py_RETURN_NONE;
}

void free_apr_pool(PyObject* capsule) {
    if (capsule == Py_None) {
        return;
    }
    apr_pool_t* mp = (apr_pool_t*)PyCapsule_GetPointer(capsule, NULL);
    if (mp && !apr_terminated) {
        apr_pool_destroy(mp);
    }
}

PyObject* make_apr_pool(PyObject* self, PyObject* args) {
    apr_pool_t* mp;
    if (apr_pool_create(&mp, NULL) != APR_SUCCESS) {
        PyErr_SetString(PyExc_TypeError, "Couldn't create memory pool");
        return NULL;
    }
    return PyCapsule_New(mp, NULL, free_apr_pool);
}

PyObject* aprdict_to_pydict(apr_pool_t* mp, apr_hash_t* ht) {
    PyObject* result = PyDict_New();
    apr_hash_index_t* hi;
    void* val;
    const void* key;

    for (hi = apr_hash_first(mp, ht); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, &key, NULL, &val);
        PyObject* pykey = PyBytes_FromString((char*)key);
        PyDict_SetItem(result, pykey, Py_True);
        Py_DECREF(pykey);
    }
    return result;
}
