#include <Python.h>

#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <malloc.h>
#define ALLOCA _malloca
#else
#include <alloca.h>
#define ALLOCA alloca
#endif
#include <assert.h>
#ifndef _WIN32
#include <unistd.h>
#else
typedef long long int64_t;
#endif

#include <apr_general.h>
#include <apr_thread_proc.h>
#include <apr_time.h>
#include <apr_hash.h>
#include <apr_strings.h>

#include "lib/helpers.h"
#include "lib/pyhelpers.h"
#include "lib/protopy.tab.h"
#include "lib/protopy.lex.h"
#include "lib/binparser.h"
#include "lib/defparser.h"
#include "lib/descriptors.h"
#include "lib/serializer.h"
#include "lib/apr_adaptor.h"


static char module_docstring[] = "Protobuf decoder and encoder.";
static char parse_docstring[] = "Parse binary Protobuf message.";
static char parse_def_docstring[] = "Parse Protobuf definition.";
static char apr_cleanup_docstring[] = "Calls apr_terminate().";
static char make_state_docstring[] = "Creates state capsule.";
static char state_ready_docstring[] = "Returns True when state finished parsing.";
static char make_apr_pool_docstring[] = "Create a new APR memory pool";
static char apr_update_hash_docstring[] = "Merge two APR hash tables";
static char apr_hash_find_docstring[] = "Retrieve a value from APR hash table";
static char make_apr_hash_docstring[] = "Create new APR hash table";
static char apr_hash_iterator_docstring[] = "Create new iterator for APR hash table";
static char apr_hash_get_docstring[] = "Fetch next pair from APR hash table";
static char apr_hash_replace_docstring[] = "Replace or add a value to APR hash table ";
static char proto_serialize_docstring[] = "Serialize Python object to Protobuf";
static char proto_describe_type_docstring[] =
        "Describe the fiels of the Python constructor of Protobuf type";

static PyObject* proto_parse(PyObject*, PyObject*);
static PyObject* proto_def_parse(PyObject*, PyObject*);
static PyObject* apr_cleanup(PyObject*, PyObject*);

static PyMethodDef module_methods[] = {
    {"proto_parse", proto_parse, METH_VARARGS, parse_docstring},
    {"proto_def_parse", proto_def_parse, METH_VARARGS, parse_def_docstring},
    {"apr_cleanup", apr_cleanup, METH_VARARGS, apr_cleanup_docstring},
    {"make_state", make_state, METH_VARARGS, make_state_docstring},
    {"state_ready", state_ready, METH_VARARGS, state_ready_docstring},
    {"make_apr_pool", make_apr_pool, METH_VARARGS, make_apr_pool_docstring},
    {"apr_update_hash", apr_update_hash, METH_VARARGS, apr_update_hash_docstring},
    {"apr_hash_find", apr_hash_find, METH_VARARGS, apr_hash_find_docstring},
    {"make_apr_hash", make_apr_hash, METH_VARARGS, make_apr_hash_docstring},
    {"apr_hash_iterator", apr_hash_iterator, METH_VARARGS, apr_hash_iterator_docstring},
    {"apr_hash_get_kv", apr_hash_get_kv, METH_VARARGS, apr_hash_get_docstring},
    {"apr_hash_replace", apr_hash_replace, METH_VARARGS, apr_hash_replace_docstring},
    {"proto_serialize", proto_serialize, METH_VARARGS, proto_serialize_docstring},
    {"describe_type", proto_describe_type, METH_VARARGS, proto_describe_type_docstring},
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
    apr_terminated = true;
    Py_RETURN_NONE;
}

static PyObject* proto_parse(PyObject* self, PyObject* args) {
    parse_state_t* state;
    PyObject* in;
    PyObject* capsule;
    
    if (!PyArg_ParseTuple(args, "OO", &in, &capsule)) {
        return NULL;
    }
    state = (parse_state_t*)PyCapsule_GetPointer(capsule, NULL);
    if (state == NULL) {
        return NULL;
    }
    Py_buffer buf;
    unsigned char* contiguous = NULL;
    if (PyObject_GetBuffer(in, &buf, PyBUF_SIMPLE)) {
        PyErr_Format(PyExc_TypeError, "%A doesn't implement buffer API", in);
        return NULL;
    }
    if (!PyBuffer_IsContiguous(&buf, 'C')) {
        if (PyBuffer_ToContiguous(contiguous, &buf, buf.len, 'C')) {
            PyErr_Format(PyExc_MemoryError, "cannot copy %i bytes from %A", buf.len, in);
            return NULL;
        }
        state->in = contiguous;
    } else {
        state->in = (unsigned char*)buf.buf;
    }
    state->len = (int64_t)buf.len;
    PyObject* result = parse_message(state);
    PyBuffer_Release(&buf);
    return result;
}

size_t available_thread_pos(parsing_progress_t* progress) {
    size_t i = 0;
    while (i < progress->nthreads) {
        if (!progress->thds_statuses[i] && !progress->thds[i]) {
            break;
        }
        i++;
    }
    return i;
}

size_t finished_thread(parsing_progress_t* progress) {
    size_t i = 0;
    while (i < progress->nthreads) {
        if (!progress->thds_statuses[i] && progress->thds[i]) {
            break;
        }
        i++;
    }
    return i;
}

void start_progress(parsing_progress_t* progress, size_t nthreads, apr_pool_t* mp) {
    size_t i = 0;

    progress->nthreads = nthreads;
    progress->thds_statuses = apr_palloc(mp, sizeof(bool) * nthreads);
    progress->thds = apr_palloc(mp, sizeof(void*) * nthreads);
    
    while (i < nthreads) {
        progress->thds_statuses[i] = false;
        progress->thds[i] = NULL;
        i++;
    }
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

void
start_defparser_thread(
    parsing_progress_t* progress,
    parse_def_args_t** thds_args,
    size_t i,
    char* source,
    apr_array_header_t* roots,
    apr_pool_t* mp) {

    parse_def_args_t* def_args = thds_args[i];

    apr_pool_t* tmp;
    apr_pool_create(&tmp, NULL);

    def_args->roots = roots;
    def_args->source = source;
    def_args->error = NULL;
    def_args->result = NULL;
    def_args->thread_id = i;
    def_args->progress = progress;
    def_args->mp = tmp;
    thds_args[i] = def_args;
    progress->thds_statuses[i] = true;
    apr_thread_create(&progress->thds[i], NULL, parse_one_def, def_args, mp);
}

apr_array_header_t* deps_from_imports(apr_array_header_t* imports, apr_pool_t* mp) {
    int i = 0;
    apr_array_header_t* result = apr_array_make(mp, 0, sizeof(char*));

    while (i < imports->nelts) {
        apr_array_header_t* elt = APR_ARRAY_IDX(imports, i, apr_array_header_t*);
        APR_ARRAY_PUSH(result, char*) = apr_array_pstrcat(mp, elt, '/');
        i++;
    }
    return result;
}

void print_messages(proto_file_t* pf) {
    int i = 0;

    // not all of these are proto-files, some are just placeholders.
    if ((int)pf == 1) {
        return;
    }
    while (i < pf->messages->nelts) {
        proto_message_t* m = APR_ARRAY_IDX(pf->messages, i, proto_message_t*);
        printf("message: %s {\n", m->t);
        int j = 0;
        while (j < m->fields->nelts) {
            proto_field_t* f = APR_ARRAY_IDX(m->fields, j, proto_field_t*);
            printf("    %s : %s = %zu\n", f->name, f->t, f->n);
            j++;
        }
        printf("}\n");
        i++;
    }

    i = 0;

    while (i < pf->enums->nelts) {
        proto_enum_t* m = APR_ARRAY_IDX(pf->enums, i, proto_enum_t*);
        printf("enum: %s {\n", m->t);
        int j = 0;
        while (j < m->members->nelts) {
            proto_enum_member_t* f = APR_ARRAY_IDX(m->members, j, proto_enum_member_t*);
            printf("    %s = %zu\n", f->name, f->n);
            j++;
        }
        printf("}\n");
        i++;
    }
}

void print_defs(apr_hash_t* defs, apr_pool_t* mp) {
    apr_hash_index_t* hi;
    proto_file_t* val;
    const void* key;

    printf("================ print_defs ================\n");
    for (hi = apr_hash_first(mp, defs); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, &key, NULL, (void**)&val);
        print_messages(val);
    }
}

const char* unrooted(apr_array_header_t* roots, const char* source) {
    int i = 0;
    while (i < roots->nelts) {
        const char* root = APR_ARRAY_IDX(roots, i, const char*);
        if (strlen(source) > strlen(root)) {
            size_t j = 0;
            while (source[j] == root[j]) {
                j++;
            }
            if (root[j] == '\0') {
                return source + j + 1;
            }

        }
        i++;
    }
    return source;
}

apr_array_header_t*
process_finished_threads(
    parsing_progress_t* progress,
    parse_def_args_t** thds_args,
    apr_hash_t* defs,
    apr_array_header_t* roots,
    error_info_t* einfo,
    apr_pool_t* mp) {

    size_t i = finished_thread(progress);
    apr_array_header_t* deps = apr_array_make(mp, 0, sizeof(char*));

    if (i < progress->nthreads) {
        if (thds_args[i]->error != NULL) {
            einfo->message = thds_args[i]->error;
            einfo->kind = thds_args[i]->error_kind;
            apr_status_t rv;
            apr_thread_join(&rv, progress->thds[i]);
            progress->thds[i] = NULL;

            size_t j;
            for (j = 0; j < progress->nthreads; j++) {
                if (progress->thds[j] != NULL) {
                    apr_thread_join(&rv, progress->thds[j]);
                }
            }
            return deps;
        }
        apr_status_t rv;
        apr_thread_join(&rv, progress->thds[i]);
        progress->thds[i] = NULL;
        proto_file_t* res = thds_args[i]->result;
        apr_pool_t* tmp = thds_args[i]->mp;
        deps = deps_from_imports(res->imports, mp);

        // We will destroy the pool where this result was allocated
        // but we need to store the result.
        res = proto_file_copy(res, mp);
        const char* ur = unrooted(roots, thds_args[i]->source);
        apr_hash_set(defs, ur, APR_HASH_KEY_STRING, res);
        apr_pool_destroy(tmp);
        // print_defs(defs, mp);
    }
    return deps;
}

void print_files(apr_hash_t* defs, apr_pool_t* mp) {
    apr_hash_index_t* hi;
    void* val;
    const char* key;

    printf("================ print_files start ================\n");
    for (hi = apr_hash_first(mp, defs); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, (void*)&key, NULL, &val);
        printf("parsed file: %s\n", key);
    }
    printf("================ print_files end ================\n");
}

static apr_hash_t*
proto_def_parse_produce(
    apr_array_header_t* sources,
    apr_array_header_t* roots,
    size_t nthreads,
    apr_hash_t* result,
    apr_pool_t* mp,
    error_info_t* einfo) {

    parsing_progress_t progress;
    size_t i = 0;
    parse_def_args_t** thds_args = ALLOCA(sizeof(parse_def_args_t*) * nthreads);

    while (i < nthreads) {
        thds_args[i] = malloc(sizeof(parse_def_args_t));
        i++;
    }

    start_progress(&progress, nthreads, mp);

    while (!apr_is_empty_array(sources) || !all_threads_finished(&progress)) {

        i = available_thread_pos(&progress);

        while (!apr_is_empty_array(sources) && i < progress.nthreads) {
            char** source = apr_array_pop(sources);
            const char* ur = unrooted(roots, *source);

            if (!apr_hash_get(result, ur, APR_HASH_KEY_STRING)) {
                apr_hash_set(result, ur, APR_HASH_KEY_STRING, (void*)true);
                start_defparser_thread(&progress, thds_args, i, *source, roots, mp);
                i = available_thread_pos(&progress);
            }
        }
        apr_array_header_t* extras = process_finished_threads(
            &progress,
            thds_args,
            result,
            roots,
            einfo,
            mp);
        if (einfo->message) {
            break;
        }
        apr_array_cat(sources, extras);
        if (apr_is_empty_array(sources) || all_threads_busy(&progress)) {
            apr_sleep(100);
        }
    }

    return result;
}

static PyObject* proto_def_parse(PyObject* self, PyObject* args) {
    PyObject* source_roots;
    PyObject* parsed_files_capsule;
    PyObject* message_ctor;
    PyObject* enum_ctor;
    PyObject* mp_capsule;
    PyObject* sources;

    if (!PyArg_ParseTuple(
            args,
            "O!O!OOOO",
            &PyList_Type,
            &sources,
            &PyList_Type,
            &source_roots,
            &parsed_files_capsule,
            &message_ctor,
            &enum_ctor,
            &mp_capsule)) {
        return NULL;
    }
    apr_hash_t* parsed_files = (apr_hash_t*)PyCapsule_GetPointer(parsed_files_capsule, NULL);
    if (!parsed_files) {
        PyErr_Format(PyExc_ValueError, "Parsed files weren't initialized");
        return NULL;
    }
    PyObject* multiprocessing = PyImport_ImportModule("multiprocessing");
    PyObject* ncores = PyObject_CallMethod(multiprocessing, "cpu_count", "");
    int nthreads;
    PyArg_Parse(ncores, "i", &nthreads);
    if (nthreads < 1) {
        nthreads = 1;
    }
    Py_DECREF(multiprocessing);
    Py_DECREF(ncores);

    apr_pool_t* mp = PyCapsule_GetPointer(mp_capsule, NULL);
    if (!mp) {
        PyErr_Format(PyExc_ValueError, "APR memory pool wasn't initialized");
        return NULL;
    }

    apr_array_header_t* roots = pylist_to_array(source_roots, mp);
    apr_hash_t* parsed_defs;

    error_info_t einfo;
    einfo.message = NULL;
    PyObject* error_class;

    apr_array_header_t* sources_arr = pylist_to_array(sources, mp);

    Py_BEGIN_ALLOW_THREADS;

    parsed_defs = proto_def_parse_produce(
        sources_arr,
        roots,
        (size_t)nthreads,
        parsed_files,
        mp,
        &einfo);

    Py_END_ALLOW_THREADS;

    if (einfo.message != NULL) {
        switch (einfo.kind) {
            case fs_error:
                error_class = PyExc_FileNotFoundError;
                break;
            case parser_error:
                // TODO(olegs): use this to set the location of the
                // syntax error: PyErr_SyntaxLocationObject
                error_class = PyExc_SyntaxError;
                break;
            default:
                error_class = PyExc_MemoryError;
                break;
        }
        PyErr_SetString(error_class, einfo.message);
        free(einfo.message);
        return NULL;
    }

    apr_hash_t* result = create_descriptors(
        parsed_defs,
        enum_ctor,
        message_ctor,
        mp);
    if (PyErr_Occurred()) {
        return NULL;
    }
    if (!result) {
        PyErr_SetString(PyExc_SystemError, "Error creating descriptors");
        return NULL;
    }
    return PyCapsule_New(result, NULL, free_apr_hash);
}
