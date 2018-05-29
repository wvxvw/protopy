#include <Python.h>
#include <apr_general.h>
#include <apr_hash.h>

#include "list.h"
#include "defparser.h"
#include "descriptors.h"
#include "pyhelpers.h"

void extract_type_name(
    const byte* tname,
    byte** pname,
    byte** package) {

    size_t len = str_size(tname) + 2;
    size_t i = len;

    while (i > 0) {
        if (tname[i] == '.' || tname[i] == ':') {
            *package = sub_str(tname, i - 2);
            *pname = sub_str(tname + i - 1, len - i - 1);
            return;
        }
        i--;
    }
    *pname = str_dup(tname);
    *package = str_dup(empty);
}

void
enum_desc(
    const byte* ftype,
    const list desc,
    apr_hash_t* const factories,
    PyObject* enum_ctor,
    apr_pool_t* const mp) {

    // TODO(olegs): In the future... we will allocate strings fro APR
    // pools, so managing their memory will be easier.
    const byte* norm_ftype = replace_str(ftype, ':', '.');
    // TODO(olegs): Maybe check if we already registered this type?

    size_t i = 0;
    PyObject* members = PyDict_New();
    apr_hash_t* mapping = apr_hash_make(mp);
    PyObject* ctor;

    list head = desc;
    list field;

    while (!null(head)) {
        field = car(head);

        byte* tname = car(field);
        size_t num = SIZE_VAL(cdr(field));
        PyObject* member = PyUnicode_FromStringAndSize(
            (char*)(tname + 2),
            str_size(tname));
        field_info_t* info = apr_palloc(mp, sizeof(field_info_t));
        size_t* key = apr_palloc(mp, sizeof(size_t));

        *key = num;
        info->n = i;
        
        apr_hash_set(mapping, key, sizeof(size_t), info);
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
    Py_DECREF(members);

    factory_t* factory = apr_palloc(mp, sizeof(factory_t));
    factory->vt_type = vt_enum;
    factory->mapping = mapping;
    factory->ctor = ctor;

    apr_hash_set(factories, ftype, str_size(ftype) + 2, factory);
}

// def message_desc(ftype, desc, factories, descriptions):
//     # TODO(olegs): We need to handle package <-> message name
//     # collision earlier
//     ftype = ftype.replace(b':', b'.')
//     if ftype in factories:
//         return

//     fields = {}
//     unresolved = {}
//     tmapping = {}
//     fields_list = []
//     fmapping = {}

//     for field in desc:
//         field_vt = field[0]
//         field_name = field[2].decode('utf-8')
//         field_type = field[1]
//         field_num = field[3]

//         if field_name not in fields:
//             fields_list.append(field_name)
//             fmapping[field_num] = len(fields_list) - 1
//             fields[field_name] = len(fields_list) - 1
//         else:
//             fmapping[field_num] = fields[field_name]
//         if field_vt == 7:
//             tmapping[field_num] = field_type
//         elif field_vt == 8:
//             tmapping[field_num] = [field_type]
//         elif field_vt == 9:
//             tmapping[field_num] = _pb_key_types[field_type[0]], field_type[1]
//         else:
//             raise Exception('Unrecognized field type: {}'.format(field_vt))

//     module, name = extract_type_name(ftype)

//     for i, f in enumerate(fields_list):
//         # TODO(olegs): it is possible that the original fields were
//         # also named pb_<somethign>
//         if f in kwlist:
//             fields_list[i] = 'pb_' + f
//         elif f[0] == '_':
//             fields_list[i] = 'pb' + f

//     result = namedtuple(name, fields_list)
//     result.__module__ = module
//     factories[ftype] = tuple([13, result, fmapping, tmapping])

void ensure_valid_names(PyObject* fields_list) {

}

void
message_desc(
    const byte* ftype,
    const list desc,
    apr_hash_t* const factories,
    PyObject* message_ctor,
    apr_pool_t* const mp) {

    byte* norm_ftype = replace_str(ftype, ':', '.');
    PyObject* fields_list = PyList_New(0);
    apr_hash_t* fields = apr_hash_make(mp);
    apr_hash_t* mapping = apr_hash_make(mp);
    list head = desc;
    size_t field_idx = 0;

    list field;
    size_t field_ast;
    byte* field_name;
    byte* field_type;
    size_t field_num;
    size_t* key;
    size_t* idx;
    byte* pytype;
    field_info_t* info;

    while (!null(head)) {
        field = car(head);
        field_ast = SIZE_VAL(field);
        field_type = STR_VAL(cdr(field));
        field_name = STR_VAL(cdr(cdr(field)));
        field_num = SIZE_VAL(cdr(cdr(cdr(field))));

        switch ((ast_type_t)field_ast) {
            case ast_field:
                // TODO(olegs): Deduplicate
                idx = apr_hash_get(fields, field_name, str_size(field_name) + 2);
                if (idx) {
                    info = apr_palloc(mp, sizeof(field_info_t));
                    info->n = (size_t)idx;
                    pytype = apr_palloc(mp, str_size(field_type) + 2);
                    memcpy(pytype, field_type, str_size(field_type) + 2);
                    info->pytype = pytype;
                    info->vt_type = vt_default;
                    key = apr_palloc(mp, sizeof(size_t));
                    *key = field_num;
                    apr_hash_set(mapping, key, sizeof(size_t), info);
                } else {
                    info = apr_palloc(mp, sizeof(field_info_t));
                    info->n = field_idx;
                    pytype = apr_palloc(mp, str_size(field_type) + 2);
                    memcpy(pytype, field_type, str_size(field_type) + 2);
                    info->pytype = pytype;
                    // We'll figure these out once we actually start parsing.
                    info->vt_type = vt_default;
                    key = apr_palloc(mp, sizeof(size_t));
                    *key = field_num;
                    apr_hash_set(mapping, key, sizeof(size_t), info);
                    apr_hash_set(fields, field_name, str_size(field_name) + 2, (void*)1);

                    PyList_Append(
                        fields_list,
                        PyUnicode_FromStringAndSize(
                            (char*)(field_name + 2),
                            str_size(field_name)));
                    field_idx++;
                }
                break;
            case ast_repeated:
                break;
            case ast_map:
                break;
            default:
                PyErr_Format(
                    PyExc_TypeError,
                    "Unrecognized field type: %i", field_ast);
                break;
        }
        head = cdr(head);
    }

    byte* name;
    byte* package;

    extract_type_name(norm_ftype, &name, &package);
    ensure_valid_names(fields_list);

    printf("creating message constructor\n");
    PyObject* ctor = PyObject_CallFunctionObjArgs(
        message_ctor,
        PyUnicode_FromStringAndSize((char*)name + 2, str_size(name)),
        fields_list,
        NULL);
    printf("creating message factory\n");
    factory_t* factory = apr_palloc(mp, sizeof(factory_t));
    factory->vt_type = vt_message;
    factory->mapping = mapping;
    factory->ctor = ctor;
    printf("message_desc finished\n");
    if (PyErr_Occurred()) {
        PyErr_Print();
    }
}

// def create_descriptors(descriptions):
//     factories = {}

//     for file, desc in descriptions.items():
//         for d in desc:
//             if d:
//                 rtype, tname, *fields = d
//                 if rtype == 0:
//                     message_desc(
//                         tname,
//                         fields,
//                         factories,
//                         descriptions,
//                     )
//                 elif rtype == 1:
//                     enum_desc(
//                         tname,
//                         fields,
//                         factories,
//                         descriptions,
//                     )

//     return factories

apr_hash_t*
create_descriptors(
    apr_hash_t* const descriptions,
    PyObject* enum_ctor,
    PyObject* message_ctor,
    apr_pool_t* const mp) {

    apr_hash_t* factories = apr_hash_make(mp);
    apr_hash_index_t* hi;
    void* val;
    const void* key;
    list file_desc;
    list desc;
    size_t rtype;
    byte* tname;
    list fields;

    for (hi = apr_hash_first(mp, descriptions); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, &key, NULL, &val);
        file_desc = (list)val;

        while (!null(file_desc)) {
            desc = LIST_VAL(file_desc);
            printf("creating descryptor for: %s\n", str(desc));
            // TODO(olegs): This shouldn't be needed, check yacc code
            // and defparser to make sure this can be removed.
            if (!null(desc)) {
                rtype = SIZE_VAL(desc);
                switch (rtype) {
                    case 0:
                        tname = STR_VAL(cdr(desc));
                        fields = cdr(cdr(desc));
                        message_desc(tname, fields, factories, message_ctor, mp);
                        break;
                    case 1:
                        tname = STR_VAL(cdr(desc));
                        fields = cdr(cdr(desc));
                        enum_desc(tname, fields, factories, enum_ctor, mp);
                        break;
                }
            }
            file_desc = cdr(file_desc);
        }
    }

    return factories;
}
