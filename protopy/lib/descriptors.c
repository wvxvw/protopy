#include <apr_general.h>

#include "list.h"

// def extract_type_name(tname):
//     i = len(tname)
//     while i > 0:
//         i -= 1
//         if tname[i] in b'.:':
//             return tname[:i].decode('utf-8'), tname[i + 1:].decode('utf-8')
//     return '', tname.decode('utf-8')

void extract_type_name(
    const byte* tname,
    byte** pname,
    byte** package,
    apr_pool_t* mp) {

}

// def enum_desc(ftype, desc, factories, descriptions):
//     ftype = ftype.replace(b':', b'.')
//     if ftype in factories:
//         return

//     members = {}
//     mmapping = {}

//     for i, field in enumerate(desc):
//         name = field[0].decode('utf-8')
//         num = field[1]

//         members[name] = i
//         mmapping[num] = i

//     result = IntEnum(ftype.decode('utf-8'), members)
//     factories[ftype] = tuple([7, result, mmapping])

void
enum_desc(
    const byte* ftype,
    const list desc,
    apr_hash_t* const factories,
    apr_hash_t* const descriptions,
    apr_pool_t* const mp) {

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

}
