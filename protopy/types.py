# -*- coding: utf-8 -*-

from collections import namedtuple
from functools import partial


_builtin_types = {
    b'int32',
    b'int64',
    b'uint32',
    b'uint64',
    b'sint32',
    b'sint64',
    b'bool',
    b'fixed64',
    b'sfixed64',
    b'double',
    b'fixed32',
    b'sfixed32',
    b'float',
    b'string',
    b'bytes',
}


def normalize_fields(fields):
    normalized_fields = []
    for f in fields:
        if f[0] == 6:      # oneof
            normalized_fields += f[2:]
        else:
            normalized_fields.append(f)
    return normalized_fields


def find_desc(name, descriptions):
    for file, desc in descriptions.items():
        print('searching in description: {}'.format(desc))
        for d in desc:
            if d and d[0] == 0:
                tname = d[1]
                fields = d[2:]
                if tname == name or tname.replace(b':', b'.') == name:
                    return normalize_fields(fields)
    # TODO(olegs): Consider lazy loading of proto descriptions.
    raise ValueError("Cannot find definition of: {}".format(name))


def tuple_from_dict(ttype, fmapping, tmapping, parser, values):
    args = [None] * len(fmapping)
    for k, v in values.items():
        t = tmapping[k]
        if t:
            args[fmapping[k]] = parser.parse(t, len(v), v)
        else:
            args[fmapping[k]] = v

    return ttype(*args)


def extract_type_name(tname):
    i = len(tname)
    while i > 0:
        i -= 1
        if tname[i] in b'.:':
            return tname[:i].decode('utf-8'), tname[i + 1:].decode('utf-8')
    return '', tname.decode('utf-8')


def message_desc(ftype, desc, factories, descriptions):
    if ftype in factories:
        return

    fields = {}
    unresolved = {}
    tmapping = {}
    fields_list = []
    fmapping = {}

    for i, field in enumerate(desc):
        print('field description: {}'.format(field))
        field_name = field[2].decode('utf-8')
        field_type = field[1]
        field_num = field[3]
        if field_type not in _builtin_types:
            print('{} not a built-in'.format(field_type))
            field_desc = factories.get(field_type, None)
            if not field_desc:
                unresolved[field_name] = field_type, field_num

        fields[field_name] = field_type, field_num
        fields_list.append(field_name)
        fmapping[field_num] = i

    module, name = extract_type_name(ftype)
    result = namedtuple(name, fields_list)
    result.__module__ = module
    factories[ftype] = result

    for field_name, (field_type, field_num) in unresolved.items():
        message_desc(
            field_type,
            find_desc(field_type, descriptions),
            factories,
            descriptions,
        )

    for field_name, (field_type, field_num) in fields.items():
        if field_type not in _builtin_types:
            tmapping[fmapping[field_num]] = factories[field_type]

    factories[ftype] = partial(tuple_from_dict, ftype, fmapping, tmapping)


def create_descriptors(descriptions):
    factories = {}

    print('creating descrtiptions from: {}'.format(descriptions))
    for file, desc in descriptions.items():
        print('processing file: {}'.format(file))
        for d in desc:
            if d:
                rtype, tname, *fields = d
                print('processing description: {}'.format(
                    (rtype, tname, fields),
                ))
                if rtype == 0:
                    message_desc(
                        tname,
                        normalize_fields(fields),
                        factories,
                        descriptions,
                    )

    return factories
