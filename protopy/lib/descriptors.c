#include <Python.h>
#include <apr_general.h>
#include <apr_hash.h>

#include "list.h"
#include "helpers.h"
#include "defparser.h"
#include "descriptors.h"
#include "pyhelpers.h"

byte* replace_str(const byte* s, byte a, byte b, apr_pool_t* mp) {
    size_t i = str_size(s) + 2;
    byte* result = apr_palloc(mp, i * sizeof(byte));

    result[0] = s[0];
    result[1] = s[1];
    while (i > 2) {
        i--;
        if (s[i] == a) {
            result[i] = b;
        } else {
            result[i] = s[i];
        }
    }
    return result;
}

void extract_type_name(
    const byte* tname,
    apr_pool_t* mp,
    char** pname,
    char** package) {

    size_t len = str_size(tname) + 2;
    size_t i = len;

    char* extracted_name;
    char* extracted_package;

    while (i > 0) {
        i--;
        if (tname[i] == '.' || tname[i] == ':') {
            extracted_package = apr_palloc(mp, (i - 1) * sizeof(char));
            memcpy(extracted_package, tname + 2, (i - 2));
            extracted_package[i - 2] = '\0';

            extracted_name = apr_palloc(mp, (len - i) * sizeof(char));
            memcpy(extracted_name, tname + i + 1, (len - i - 1));
            extracted_name[len - i - 1] = '\0';
            *pname = extracted_name;
            *package = extracted_package;
            return;
        }
    }
    *pname = bytes_cstr(tname, mp);
    *package = dupstr("", mp);
}

void
enum_desc(
    const byte* ftype,
    const list_t* desc,
    apr_hash_t* const factories,
    PyObject* enum_ctor,
    apr_pool_t* const mp) {

    byte* norm_ftype = replace_str(ftype, ':', '.', mp);
    // TODO(olegs): Maybe check if we already registered this type?

    size_t i = 0;
    PyObject* members = PyDict_New();
    PyObject* ctor;

    list_t* head = (list_t*)desc;
    list_t* field;

    while (!null(head)) {
        field = car(head);

        byte* tname = STR_VAL(field);
        size_t num = SIZE_VAL(cdr(field));
        PyObject* member = PyUnicode_FromStringAndSize(
            (char*)(tname + 2),
            str_size(tname));
        
        PyDict_SetItem(members, member, PyLong_FromSsize_t(num));

        head = cdr(head);
        i++;
    }
    ctor = PyObject_CallFunctionObjArgs(
        enum_ctor,
        PyUnicode_FromStringAndSize(
            (char*)(norm_ftype + 2),
            str_size(norm_ftype)),
        members,
        NULL);
    if (PyErr_Occurred()) {
        return;
    }
    Py_INCREF(ctor);

    factory_t* factory = apr_palloc(mp, sizeof(factory_t));
    factory->vt_type = vt_enum;
    factory->ctor = ctor;

    apr_hash_set(factories, bytes_cstr(norm_ftype, mp), APR_HASH_KEY_STRING, factory);
}

field_info_t*
add_field_info(
    byte* field_type,
    size_t field_num,
    size_t idx,
    apr_hash_t* mapping,
    apr_pool_t* mp) {

    field_info_t* info = apr_palloc(mp, sizeof(field_info_t));
    info->n = idx;
    // field_type and field_num come from recently parsed description
    // which will be discarded as soon as parsing finishes, this is
    // why we need to copy them.
    info->pytype = bytes_cstr(field_type, mp);
    info->vt_type = vt_default;

    size_t* key = apr_palloc(mp, sizeof(size_t));
    *key = field_num;
    apr_hash_set(mapping, key, sizeof(size_t), info);
    return info;
}

void add_pyfield(PyObject* fields, byte* field_name, apr_hash_t* keywords, apr_pool_t* mp) {
    size_t len = str_size(field_name);
    char* fname;
    PyObject* new_name;

    if (apr_hash_get(keywords, bytes_cstr(field_name, mp), APR_HASH_KEY_STRING)) {
        fname = malloc(len + 4);
        memcpy(fname, "pb_", 3);
        memcpy(fname + 3, field_name + 2, len);
        fname[len + 3] = '\0';
        new_name = PyUnicode_FromString(fname);
    } else {
        fname = (char*)(field_name + 2);
        new_name = PyUnicode_FromStringAndSize(fname, len);
    }
    // TODO(olegs): Here and in message_desc: revisit the Py_INCREF()
    // calls and make sure they are actually needed (I think they
    // aren't needed).
    Py_INCREF(new_name);
    PyList_Append(fields, new_name);
}

void
message_desc(
    const byte* ftype,
    const list_t* desc,
    apr_hash_t* const factories,
    PyObject* message_ctor,
    apr_hash_t* const keywords,
    apr_pool_t* const mp) {

    byte* norm_ftype = replace_str(ftype, ':', '.', mp);
    PyObject* fields_list = PyList_New(0);
    apr_hash_t* mapping = apr_hash_make(mp);
    list_t* head = (list_t*)desc;
    size_t field_idx = 0;

    list_t* field;
    size_t field_ast;
    byte* field_name;
    byte* field_type;
    size_t field_num;
    field_info_t* info;
    list_t* kv_type;

    while (!null(head)) {
        field = car(head);
        field_ast = SIZE_VAL(field);
        field_name = STR_VAL(cdr(cdr(field)));
        field_num = SIZE_VAL(cdr(cdr(cdr(field))));

        switch ((ast_type_t)field_ast) {
            case ast_field:
                field_type = STR_VAL(cdr(field));
                add_field_info(field_type, field_num, field_idx, mapping, mp);
                add_pyfield(fields_list, field_name, keywords, mp);
                field_idx++;
                break;
            case ast_repeated:
                field_type = STR_VAL(cdr(field));
                info = add_field_info(field_type, field_num, field_idx, mapping, mp);

                info->vt_type = vt_repeated;
                info->extra_type_info.elt = vt_default;

                add_pyfield(fields_list, field_name, keywords, mp);
                field_idx++;
                break;
            case ast_map:
                kv_type = LIST_VAL(cdr(field));
                field_type = empty;
                info = add_field_info(field_type, field_num, field_idx, mapping, mp);

                info->vt_type = vt_map;
                info->extra_type_info.pair.key = (vt_type_t)SIZE_VAL(kv_type);
                info->extra_type_info.pair.val = vt_default;
                // TODO(olegs): Why not store this in field_type?
                info->extra_type_info.pair.pyval = bytes_cstr(STR_VAL(cdr(kv_type)), mp);
                add_pyfield(fields_list, field_name, keywords, mp);
                field_idx++;
                break;
            default:
                PyErr_Format(
                    PyExc_TypeError,
                    "Unrecognized field type: %i", field_ast);
                return;
        }
        head = cdr(head);
    }

    char* name;
    char* package;

    extract_type_name(norm_ftype, mp, &name, &package);

    Py_INCREF(fields_list);
    PyObject* args = PyTuple_New(2);
    PyObject* arg1 = PyUnicode_FromString(name);
    Py_INCREF(arg1);
    PyTuple_SetItem(args, 0, arg1);
    PyTuple_SetItem(args, 1, fields_list);
    PyObject* ctor = PyObject_Call(message_ctor, args, NULL);
    Py_DECREF(args);
    if (!ctor) {
        PyErr_Format(
            PyExc_TypeError,
            "Couldn't create constructor of %A, %s",
            message_ctor,
            name);
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

    apr_hash_set(factories, bytes_cstr(norm_ftype, mp), APR_HASH_KEY_STRING, factory);
}

apr_hash_t* pylist_to_apr_hash(PyObject* pylist, apr_pool_t* mp) {
    apr_hash_t* result = apr_hash_make(mp);
    Py_ssize_t len = PyList_Size(pylist);

    while (len > 0) {
        len--;
        PyObject* str = PyList_GetItem(pylist, len);
        char* key = PyUnicode_AsUTF8(str);
        apr_hash_set(result, key, APR_HASH_KEY_STRING, (void*)1);
    }
    return result;
}

apr_hash_t*
create_descriptors(
    apr_hash_t* const descriptions,
    PyObject* enum_ctor,
    PyObject* message_ctor,
    PyObject* pykeywords,
    apr_pool_t* const mp) {

    apr_hash_t* factories = apr_hash_make(mp);
    // TODO(olegs): Implemented is_keyword() in a way we don't need
    // to create this hash-table every time.
    apr_hash_t* keywords = pylist_to_apr_hash(pykeywords, mp);
    apr_hash_index_t* hi;
    void* val;
    const void* key;
    list_t* file_desc;
    list_t* desc;
    size_t rtype;
    byte* tname;
    list_t* fields;

    for (hi = apr_hash_first(mp, descriptions); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, &key, NULL, &val);
        file_desc = (list_t*)val;

        while (!null(file_desc)) {
            desc = LIST_VAL(file_desc);
            rtype = SIZE_VAL(desc);
            switch (rtype) {
                case 0:
                    tname = STR_VAL(cdr(desc));
                    fields = cdr(cdr(desc));
                    message_desc(tname, fields, factories, message_ctor, keywords, mp);
                    break;
                case 1:
                    tname = STR_VAL(cdr(desc));
                    fields = cdr(cdr(desc));
                    enum_desc(tname, fields, factories, enum_ctor, mp);
                    break;
            }
            if (PyErr_Occurred()) {
                return NULL;
            }
            file_desc = cdr(file_desc);
        }
    }

    return factories;
}
