# -*- coding: utf-8 -*-

from collections import namedtuple
from functools import partial
from enum import Enum


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

_pb_types = {
    b'int32': 0,
    b'int64': 1,
    b'uint32': 2,
    b'uint64': 3,
    b'sing32': 4,
    b'sing64': 5,
    b'bool': 6,
    # 'enum': 7,
    b'fixed64': 8,
    b'sfixed64': 9,
    b'double': 10,
    b'string': 11,
    b'bytes': 12,
    # 'message': 13,
    # 'repeated': 14,
    b'fixed32': 15,
    b'sfixed32': 16,
}


def value_type(pbtype, factory):
    result = _pb_types.get(pbtype, None)
    if result is None:
        if factory[0] == tuple_from_dict:
            return 13
        if factory[0] == enum_from_dict:
            return 7
        return 14
    return result


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


def tuple_from_dict(ftype, factories, descriptions, values):
    _, ttype, fmapping, tmapping = factories[ftype]
    print('tuple_from_dict: {}, {}'.format(ttype, fmapping))
    args = [None] * len(fmapping)

    for k, v in values.items():
        t = tmapping[k]
        if t in _builtin_types:
            args[fmapping[k]] = v
        else:
            # TODO(olegs): This could be an enum or a list
            args[fmapping[k]] = tuple_from_dict(
                descriptions[t],
                factories,
                descriptions,
                v,
            )

    return ttype(*args)


def enum_from_dict(ttype, mmaping, parser, value):
    return ttype(mmaping[value])


def extract_type_name(tname):
    i = len(tname)
    while i > 0:
        i -= 1
        if tname[i] in b'.:':
            return tname[:i].decode('utf-8'), tname[i + 1:].decode('utf-8')
    return '', tname.decode('utf-8')


def enum_desc(ftype, desc, factories, descriptions):
    if ftype in factories:
        return

    members = []
    mmapping = {}

    for i, field in enumerate(desc):
        print('field description: {}'.format(field))
        name = field[0].decode('utf-8')
        num = field[1]

        members.append(name)
        mmapping[num] = i

    result = Enum(ftype.decode('utf-8'), members)
    factories[ftype] = tuple([enum_from_dict, result, mmapping])


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
        tmapping[field_num] = field_type
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

    factories[ftype] = tuple([tuple_from_dict, result, fmapping, tmapping])


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
