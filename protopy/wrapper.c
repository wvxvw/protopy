#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <assert.h>

#include <Python.h>
#include <apr_general.h>
#include <apr_thread_proc.h>

#include "lib/helpers.h"
#include "lib/pyhelpers.h"
#include "lib/protopy.lex.h"
#include "lib/protopy.tab.h"
#include "lib/binparser.h"
#include "lib/defparser.h"
#include "lib/list.h"


static char module_docstring[] = "Protobuf decoder and encoder.";
static char parse_docstring[] = "Parse binary Protobuf message.";
static char parse_def_docstring[] = "Parse Protobuf definition.";
static char apr_cleanup_docstring[] = "Calls apr_terminate().";
static char make_state_docstring[] = "Creates state capsule.";
static char state_ready_docstring[] = "Returns True when state finished parsing.";
static char state_result_docstring[] = "The object that was deserialized.";

static PyObject* proto_parse(PyObject* self, PyObject* args);
static PyObject* proto_def_parse(PyObject* self, PyObject* args);
static PyObject* apr_cleanup(PyObject* self, PyObject* args);
static PyObject* make_state(PyObject* self, PyObject* args);
static PyObject* state_ready(PyObject* self, PyObject* args);
static PyObject* state_result(PyObject* self, PyObject* args);

static PyMethodDef module_methods[] = {
    {"proto_parse", proto_parse, METH_VARARGS, parse_docstring},
    {"proto_def_parse", proto_def_parse, METH_VARARGS, parse_def_docstring},
    {"apr_cleanup", apr_cleanup, METH_VARARGS, apr_cleanup_docstring},
    {"make_state", make_state, METH_VARARGS, make_state_docstring},
    {"state_ready", state_ready, METH_VARARGS, state_ready_docstring},
    {"state_result", state_result, METH_VARARGS, state_result_docstring},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef protopy_module = {
    PyModuleDef_HEAD_INIT,
    "protopy.wrapped",
    module_docstring, /* m_doc */
    -1, /* m_size */
    module_methods /* m_methods */
};

PyMODINIT_FUNC PyInit_wrapped(void);
PyMODINIT_FUNC PyInit_wrapped(void) {
    Py_Initialize();
    apr_initialize();
    return PyModule_Create(&protopy_module);
}

static PyObject* apr_cleanup(PyObject* self, PyObject* args) {
    apr_terminate();
    return Py_None;
}

static void free_state(PyObject* capsule) {
    if (capsule == Py_None) {
        return;
    }
    parse_state* state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    del(state->in);
    printf("freeing the state %p\n", state);
    free(state);
    printf("state freed\n");
}

static PyObject* state_ready(PyObject* self, PyObject* args) {
    PyObject* capsule;
    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    parse_state* state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (PyErr_Occurred()) {
        return NULL;
    }
    if (state->out == Py_None) {
        return Py_False;
    }
    return Py_True;
}

static PyObject* state_result(PyObject* self, PyObject* args) {
    PyObject* capsule;
    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    parse_state* state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (PyErr_Occurred()) {
        return NULL;
    }
    return state->out;
}

static PyObject* make_state(PyObject* self, PyObject* args) {
    parse_state* state = malloc(sizeof(parse_state));
    state->in = nil;
    state->pos = 0;
    state->out = Py_None;
    printf("allocating new state %p\n", state);
    return PyCapsule_New(state, NULL, free_state);
}

static PyObject* proto_parse(PyObject* self, PyObject* args) {
    parse_state* state;
    char* in;
    size_t available;
    PyObject* capsule;
    
    if (!PyArg_ParseTuple(args, "s#O", &in, &available, &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }
    state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (state == NULL) {
        return NULL;
    }
    state->in = cons(strdup(in), tstr, state->in);
    size_t parsed = parse(state);
    printf("parsed bytes: %zu\n", parsed);
    printf("produced result: %p\n", state->out);
    PyObject_Print(state->out, stdout, 0);
    printf("\n");
    return state->out;
}

static PyObject* proto_def_parse(PyObject* self, PyObject* args) {
    PyObject* source_roots;
    PyObject* parsed_files;
    char* source;
    
    if (!PyArg_ParseTuple(
            args,
            "yO!O!",
            &source,
            &PyList_Type,
            &source_roots,
            &PyDict_Type,
            &parsed_files)) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        }
        return NULL;
    }

    PyObject* multiprocessing = PyImport_ImportModule("multiprocessing");
    print_obj("imported multiprocessing: %s\n", multiprocessing);
    PyObject* ncores = PyObject_CallMethod(multiprocessing, "cpu_count", "");
    print_obj("multiprocessing.cpu_count: %s\n", ncores);
    int nthreads;
    PyArg_Parse(ncores, "i", &nthreads);
    if (nthreads < 1) {
        nthreads = 1;
    }
    nthreads = 1;
    printf("nthreads: %d\n", nthreads);
    apr_status_t rv;
    apr_pool_t* mp = NULL;
    apr_thread_t** thd_arr = alloca(nthreads * sizeof(void*));
    parse_def_args* thd_args = malloc(nthreads * sizeof(parse_def_args));
    apr_threadattr_t* thd_attr;
    int i, errors = 0;

    Py_BEGIN_ALLOW_THREADS;

    if (apr_pool_create(&mp, NULL) != APR_SUCCESS) {
        PyErr_SetString(PyExc_TypeError, "Couldn't create memory pool");
        errors = 1;
    }

    if (errors == 0 && apr_threadattr_create(&thd_attr, mp) != APR_SUCCESS) {
        PyErr_SetString(PyExc_TypeError, "Couldn't create thread attribute");
        errors = 1;
    }

    if (errors == 0) {
        for (i = 0; i < nthreads; i++) {
            thd_args[i].source = source;
            thd_args[i].error = "";
            thd_args[i].result = NULL;
            rv = apr_thread_create(
                &thd_arr[i],
                thd_attr,
                parse_one_def,
                &thd_args[i],
                mp);
            if (rv != APR_SUCCESS) {
                errors = 1;
                nthreads = i;
                break;
            }
        }
    } else {
        nthreads = 0;
    }

    for (i = 0; i < nthreads; i++) {
        rv = apr_thread_join(&rv, thd_arr[i]);
        if (rv != APR_SUCCESS) {
            errors++;
        }
    }
    if (errors > 0) {
        PyErr_SetString(PyExc_TypeError, "Error running threads");
    }

    if (mp != NULL) {
        apr_pool_destroy(mp);
    }
    
    Py_END_ALLOW_THREADS;

    printf("all arp threads finished\n");

    for (i = 0; i < nthreads; i++) {
        if (strcmp(thd_args[i].error, "")) {
            PyErr_SetString(PyExc_TypeError, thd_args[i].error);
            return NULL;
        } else {
            printf("parsed raw result: %p\n", thd_args[i].result);
            printf("parsed raw result: %s\n", str(thd_args[i].result));
            list maybe_unparsed = imports(thd_args[i].result);
            printf("maybe_unparsed: %s\n", str(maybe_unparsed));
            print_obj("parsed AST: %s\n", list_to_pylist(thd_args[i].result));
        }
    }

    if (thd_args != NULL) {
        free(thd_args);
    }

    return Py_None;
}
