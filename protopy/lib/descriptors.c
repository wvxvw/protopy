#include <Python.h>
#include <apr_general.h>
#include <apr_hash.h>
#include <apr_strings.h>

#include "helpers.h"
#include "defparser.h"
#include "descriptors.h"
#include "pyhelpers.h"


void extract_type_name(
    const char* tname,
    apr_pool_t* mp,
    char** pname,
    char** package) {

    size_t len = strlen(tname);
    size_t i = len;

    char* extracted_name;
    char* extracted_package;

    while (i > 0) {
        i--;
        if (tname[i] == '.') {
            extracted_package = apr_palloc(mp, (i + 1) * sizeof(char));
            memcpy(extracted_package, tname, i);
            extracted_package[i] = '\0';

            extracted_name = apr_palloc(mp, (len - i) * sizeof(char));
            memcpy(extracted_name, tname + i + 1, (len - i - 1));
            extracted_name[len - i - 1] = '\0';
            *pname = extracted_name;
            *package = extracted_package;
            return;
        }
    }
    *pname = apr_pstrdup(mp, tname);
    *package = apr_pstrdup(mp, "");
}

void
enum_desc(
    const proto_enum_t* desc,
    apr_hash_t* const factories,
    PyObject* enum_ctor,
    apr_pool_t* const mp) {

    // TODO(olegs): Maybe check if we already registered this type?
    int i = 0;
    PyObject* members = PyDict_New();

    while (i < desc->members->nelts) {
        proto_enum_member_t* field = APR_ARRAY_IDX(desc->members, i, proto_enum_member_t*);
        PyObject* member = PyUnicode_FromString(mdupstr(field->name));
        
        PyDict_SetItem(members, member, PyLong_FromSsize_t(field->n));
        i++;
    }
    PyObject* ctor = PyObject_CallFunctionObjArgs(
        enum_ctor,
        PyUnicode_FromString(mdupstr(desc->t)),
        members,
        NULL);
    if (PyErr_Occurred()) {
        return;
    }
    Py_INCREF(ctor);

    factory_t* factory = apr_palloc(mp, sizeof(factory_t));
    factory->vt_type = vt_enum;
    factory->ctor = ctor;

    apr_hash_set(factories, desc->t, APR_HASH_KEY_STRING, factory);
}

field_info_t*
add_field_info(
    const char* field_type,
    size_t field_num,
    size_t idx,
    apr_hash_t* mapping,
    apr_pool_t* mp) {

    field_info_t* info = apr_palloc(mp, sizeof(field_info_t));
    info->n = idx;
    info->pytype = field_type;
    info->vt_type = vt_default;

    size_t* key = apr_palloc(mp, sizeof(size_t));
    *key = field_num;
    apr_hash_set(mapping, key, sizeof(size_t), info);
    return info;
}

void add_pyfield(PyObject* fields, const char* field_name, apr_pool_t* mp) {
    size_t len = strlen(field_name);
    PyObject* new_name;

    if (is_keyword(field_name)) {
        char* fname = malloc(len + 4);
        memcpy(fname, "pb_", 3);
        memcpy(fname + 3, field_name, len);
        fname[len + 3] = '\0';
        new_name = PyUnicode_FromString(mdupstr(fname));
    } else {
        new_name = PyUnicode_FromStringAndSize(mdupstr(field_name), len);
    }
    // TODO(olegs): Here and in message_desc: revisit the Py_INCREF()
    // calls and make sure they are actually needed (I think they
    // aren't needed).
    Py_INCREF(new_name);
    PyList_Append(fields, new_name);
}

void
message_desc(
    const proto_message_t* desc,
    apr_hash_t* const factories,
    PyObject* message_ctor,
    apr_pool_t* const mp) {

    PyObject* fields_list = PyList_New(0);
    apr_hash_t* mapping = apr_hash_make(mp);

    int i = 0;
    int j = 0;
    int k = 0;

    while (i < desc->fields->nelts) {
        proto_field_t* field = APR_ARRAY_IDX(desc->fields, i, proto_field_t*);
        add_field_info(field->t, field->n, i, mapping, mp);
        add_pyfield(fields_list, field->name, mp);
        i++;
    }
    while (j < desc->repeated->nelts) {
        proto_field_t* field = APR_ARRAY_IDX(desc->repeated, j, proto_field_t*);
        field_info_t* info = add_field_info(field->t, field->n, j + i, mapping, mp);
        info->vt_type = vt_repeated;
        info->extra_type_info.elt = vt_default;
        add_pyfield(fields_list, field->name, mp);
        j++;
    }
    while (k < desc->maps->nelts) {
        proto_map_field_t* map = APR_ARRAY_IDX(desc->maps, k, proto_map_field_t*);
        const char* empty = apr_pstrdup(mp, "");
        field_info_t* info = add_field_info(empty, map->n, k + j + i, mapping, mp);
        info->vt_type = vt_map;
        info->extra_type_info.pair.key = map->kt;
        info->extra_type_info.pair.val = vt_default;
        // TODO(olegs): Why not store this in field_type?
        info->extra_type_info.pair.pyval = map->vt;
        add_pyfield(fields_list, map->name, mp);
        k++;
    }

    char* name;
    char* package;

    extract_type_name(desc->t, mp, &name, &package);

    Py_INCREF(fields_list);
    PyObject* args = PyTuple_New(2);
    PyObject* arg1 = PyUnicode_FromString(mdupstr(name));
    Py_INCREF(arg1);
    PyTuple_SetItem(args, 0, arg1);
    PyTuple_SetItem(args, 1, fields_list);
    PyObject* ctor = PyObject_Call(message_ctor, args, NULL);
    Py_DECREF(args);

    if (!ctor) {
        PyObject *t, *v, *tb;

        PyErr_Fetch(&t, &v, &tb);
        PyErr_Format(
            PyExc_TypeError,
            "Couldn't create constructor for %s, using %A\ncause: %A",
            name,
            message_ctor,
            v);
        return;
    }
    PyObject_SetAttrString(
        ctor,
        "__module__",
        PyUnicode_FromString(package));

    factory_t* factory = apr_palloc(mp, sizeof(factory_t));
    factory->vt_type = vt_message;
    factory->mapping = mapping;
    factory->ctor = ctor;

    apr_hash_set(factories, desc->t, APR_HASH_KEY_STRING, factory);
}

apr_hash_t*
create_descriptors(
    apr_hash_t* const descriptions,
    PyObject* enum_ctor,
    PyObject* message_ctor,
    apr_pool_t* const mp) {

    apr_hash_t* factories = apr_hash_make(mp);
    apr_hash_index_t* hi;
    proto_file_t* pfile;
    const void* key;

    for (hi = apr_hash_first(mp, descriptions); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, &key, NULL, (void**)(&pfile));

        int i = 0;
        while (i < pfile->enums->nelts) {
            proto_enum_t* e = APR_ARRAY_IDX(pfile->enums, i, proto_enum_t*);
            enum_desc(e, factories, enum_ctor, mp);
            if (PyErr_Occurred()) {
                return NULL;
            }
            i++;
        }
        i = 0;
        while (i < pfile->messages->nelts) {
            proto_message_t* e = APR_ARRAY_IDX(pfile->messages, i, proto_message_t*);
            message_desc(e, factories, message_ctor, mp);
            if (PyErr_Occurred()) {
                return NULL;
            }
            i++;
        }
    }

    return factories;
}
