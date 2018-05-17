#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <assert.h>
#include <unistd.h>

#include <Python.h>
#include <apr_general.h>
#include <apr_thread_proc.h>
#include <apr_time.h>
#include <apr_hash.h>

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
static char state_set_factory_docstring[] = "Specify the message class and dict "\
                                            "of definitions to use when parsing "\
                                            "binary messages stream.";

static PyObject* proto_parse(PyObject* self, PyObject* args);
static PyObject* proto_def_parse(PyObject* self, PyObject* args);
static PyObject* apr_cleanup(PyObject* self, PyObject* args);

static PyMethodDef module_methods[] = {
    {"proto_parse", proto_parse, METH_VARARGS, parse_docstring},
    {"proto_def_parse", proto_def_parse, METH_VARARGS, parse_def_docstring},
    {"apr_cleanup", apr_cleanup, METH_VARARGS, apr_cleanup_docstring},
    {"make_state", make_state, METH_VARARGS, make_state_docstring},
    {"state_ready", state_ready, METH_VARARGS, state_ready_docstring},
    {"state_result", state_result, METH_VARARGS, state_result_docstring},
    {"state_set_factory", state_set_factory, METH_VARARGS, state_set_factory_docstring},
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

static PyObject* proto_parse(PyObject* self, PyObject* args) {
    parse_state* state;
    char* in;
    int available;
    PyObject* capsule;
    
    if (!PyArg_ParseTuple(args, "s#O", &in, &available, &capsule)) {
        return NULL;
    }
    state = (parse_state*)PyCapsule_GetPointer(capsule, NULL);
    if (state == NULL) {
        return NULL;
    }
    PyObject* parsed = parse_message(state, in, (size_t)available);
    return parsed;
}

size_t available_thread_pos(parsing_progress_t* progress) {
    size_t i = 0;
    while (i < progress->nthreads) {
        if (!progress->thds_statuses[i]) {
            break;
        }
        i++;
    }
    return i;
}

size_t finished_thread(parsing_progress_t* progress) {
    size_t i = 0;
    while (i < progress->nthreads) {
        if (!progress->thds_statuses[i] && progress->thds[i] != NULL) {
            break;
        }
        i++;
    }
    return i;
}

void start_progress(parsing_progress_t* progress, size_t nthreads, apr_pool_t* mp) {
    size_t i = 0;

    progress->nthreads = nthreads;
    progress->thds_statuses = malloc(sizeof(bool) * nthreads);
    progress->thds = malloc(sizeof(void*) * nthreads);
    
    while (i < nthreads) {
        progress->thds_statuses[i] = false;
        progress->thds[i] = NULL;
        i++;
    }
}

void finish_progress(parsing_progress_t* progress) {
    free(progress->thds_statuses);
    free(progress->thds);
}

bool all_threads_finished(parsing_progress_t* progress) {
    size_t i = 0;
    while (i < progress->nthreads) {
        if (progress->thds_statuses[i] || progress->thds[i] != NULL) {
            return false;
        }
        i++;
    }
    return true;
}

bool all_threads_busy(parsing_progress_t* progress) {
    return available_thread_pos(progress) < progress->nthreads;
}

static apr_hash_t*
proto_def_parse_produce(list sources, list roots, size_t nthreads, apr_pool_t* mp) {
    parsing_progress_t progress;
    size_t i = 0;
    parse_def_args_t** thds_args = alloca(sizeof(parse_def_args_t*) * nthreads);
    apr_hash_t* result = apr_hash_make(mp);

    while (i < nthreads) {
        thds_args[i] = malloc(sizeof(parse_def_args_t));
        i++;
    }

    start_progress(&progress, nthreads, mp);

    while (!null(sources) || !all_threads_finished(&progress)) {

        i = available_thread_pos(&progress);

        if (i < progress.nthreads && !null(sources)) {
            parse_def_args_t* def_args = thds_args[i];

            def_args->roots = roots;
            // TODO(olegs): This should be a separate function
            size_t tocopy = str_size((byte*)car(sources));
            char* source = malloc((tocopy + 1) * sizeof(char));
            memcpy(source, (char*)car(sources) + 2, tocopy);
            source[tocopy] = '\0';
            def_args->source = source;
            def_args->error = "";
            def_args->result = NULL;
            def_args->thread_id = i;
            def_args->progress = &progress;
            thds_args[i] = def_args;
            progress.thds_statuses[i] = true;
            apr_thread_create(&progress.thds[i], NULL, parse_one_def, def_args, mp);

            sources = cdr(sources);
        } else {
            i = finished_thread(&progress);
            if (i < progress.nthreads) {
                // TODO(olegs): Do something if the thread reported an error.
                if (strcmp(thds_args[i]->error, "")) {
                    printf("Encountered error reading sources: %s\n", thds_args[i]->error);
                    // TODO(olegs): We cannot throw here because we don't hold GIL
                    // PyErr_Format(PyExc_TypeError, "%s", thds_args[i]->error);
                    del(sources);
                    sources = nil;
                    apr_status_t rv;
                    apr_thread_join(&rv, progress.thds[i]);
                    progress.thds[i] = NULL;
                    // TODO(olegs): We still need to join all remaining threads
                    // before we can clean up.
                    break;
                }
                apr_status_t rv;
                apr_thread_join(&rv, progress.thds[i]);
                progress.thds[i] = NULL;
                list deps = imports(thds_args[i]->result);
                list new_sources = append(sources, deps);
                del(sources);
                del(deps);
                sources = new_sources;
                apr_hash_set(
                    result,
                    thds_args[i]->source,
                    strlen(thds_args[i]->source),
                    normalize_messages(normalize_types(thds_args[i]->result)));
            } else if (null(sources) || all_threads_busy(&progress)) {
                apr_sleep(100);
            }
        }
    }

    finish_progress(&progress);

    return result;
}

PyObject* aprdict_to_pydict(apr_pool_t* mp, apr_hash_t* ht) {
    PyObject* result = PyDict_New();
    apr_hash_index_t* hi;
    void* val;
    const void* key;

    for (hi = apr_hash_first(mp, ht); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, &key, NULL, &val);
        PyObject* pykey = Py_BuildValue("y", (char*)key);
        PyDict_SetItem(result, pykey, list_to_pylist((list)val));
    }
    return result;
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
        return NULL;
    }

    PyObject* multiprocessing = PyImport_ImportModule("multiprocessing");
    PyObject* ncores = PyObject_CallMethod(multiprocessing, "cpu_count", "");
    int nthreads;
    PyArg_Parse(ncores, "i", &nthreads);
    if (nthreads < 1) {
        nthreads = 1;
    }
    list roots = pylist_to_list(source_roots);
    apr_pool_t* mp = NULL;
    apr_hash_t* parsed_defs;

    if (apr_pool_create(&mp, NULL) != APR_SUCCESS) {
        PyErr_SetString(PyExc_TypeError, "Couldn't create memory pool");
        return NULL;
    }

    Py_BEGIN_ALLOW_THREADS;

    parsed_defs = proto_def_parse_produce(
        cons_str(source, strlen(source), nil),
        roots,
        (size_t)nthreads,
        mp);

    del(roots);

    Py_END_ALLOW_THREADS;

    PyObject* types = PyImport_ImportModule("protopy.types");
    PyObject* description = aprdict_to_pydict(mp, parsed_defs);
    PyObject* result = PyObject_CallMethod(
        types,
        "create_descriptors",
        "O",
        description);
    PyObject* keys = PyDict_Keys(description);
    Py_ssize_t length = PyList_Size(keys);
    Py_ssize_t i = 0;

    while (i < length) {
        PyDict_SetItem(parsed_files, PyList_GetItem(keys, i), Py_True);
        i++;
    }
    Py_DECREF(description);

    apr_pool_destroy(mp);

    return result;
}
