#include <Python.h>
#include <apr_general.h>
#include <apr_hash.h>

#include "list.h"
#include "descriptors.h"

void extract_type_name(
    const byte* tname,
    byte** pname,
    byte** package) {

    size_t len = str_size(tname) + 2;
    size_t i = len;

    while (i > 0) {
        if (tname[i] == '.' || tname[i] == ':') {
            *package = sub_str(tname, i - 2);
            *pname = sub_str(tname + i, len - i);
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

    const byte* norm_ftype = replace_str(ftype, ':', '.');

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
        field_info_t* info = apr_palloc(mp, sizeof(field));
        const void* key = apr_palloc(mp, sizeof(size_t));

        *(size_t*)key = num;
        *(size_t*)info->n = i;
        
        apr_hash_set(mapping, key, sizeof(size_t), info);
        PyDict_SetItem(members, member, PyLong_FromSsize_t(num));
        
        head = cdr(head);
        i++;
    }
    ctor = PyObject_CallFunctionObjArgs(
        enum_ctor,
        PyUnicode_FromStringAndSize(
            (char*)norm_ftype + 2,
            str_size(norm_ftype)),
        members,
        NULL);

    factory_t* factory = apr_palloc(mp, sizeof(factory_t));
    *(size_t*)factory->vt_type = vt_enum;
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

void
message_desc(
    const byte* ftype,
    const list desc,
    apr_hash_t* const factories,
    apr_hash_t* const descriptions,
    apr_pool_t* const mp) {

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
    const apr_hash_t* const descriptions,
    apr_pool_t* const mp) {

    apr_hash_t* result = apr_hash_make(mp);

    return result;
}
