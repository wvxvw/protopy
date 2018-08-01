#ifndef APR_ADAPTOR_H_
#define APR_ADAPTOR_H_

#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef PyObject* (*apr_to_py_fn_t)(void*);

PyObject* make_apr_pool(PyObject*, PyObject*);
PyObject* apr_update_hash(PyObject*, PyObject*);
PyObject* apr_hash_find(PyObject*, PyObject*);
PyObject* make_apr_hash(PyObject*, PyObject*);
PyObject* apr_hash_iterator(PyObject*, PyObject*);
PyObject* apr_hash_get_kv(PyObject*, PyObject*);
PyObject* apr_hash_replace(PyObject*, PyObject*);
PyObject* aprdict_to_pydict(apr_pool_t*, apr_hash_t*);
PyObject* proto_describe_type(PyObject*, PyObject*);
void free_apr_hash(PyObject*);

extern bool apr_terminated;

#ifdef __cplusplus
}
#endif

#endif // APR_ADAPTOR_H_
