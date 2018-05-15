# -*- coding: utf-8 -*-

from collections import namedtuple
from functools import partial
from enum import IntEnum


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
    b'sint32': 4,
    b'sint64': 5,
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


def is_builtin(pbtype):
    return pbtype in _pb_types


def value_type(pbtype, factory):
    if type(pbtype) == list:
        return 14
    result = _pb_types.get(pbtype, None)
    if result is None:
        if factory[0] == tuple_from_dict:
            return 13
        if factory[0] == enum_from_dict:
            return 7
    # this means error
    if result is None:
        result = 17
    return result


# TODO(olegs): This should happen in defparser
def normalize_fields(fields):
    normalized_fields = []
    for f in fields:
        if f[0] == 6:      # oneof
            fname = f[1]
            normalized_fields += [[7, x[1], fname] + x[3:] for x in f[2:]]
        else:
            normalized_fields.append(f)
    return normalized_fields


def find_desc(name, descriptions):
    for file, desc in descriptions.items():
        for d in desc:
            if d and d[0] in [0, 1]:  # message or enum
                tname = d[1]
                fields = d[2:]
                if tname == name or tname.replace(b':', b'.') == name:
                    return normalize_fields(fields)
    # TODO(olegs): Consider lazy loading of proto descriptions.
    raise ValueError("Cannot find definition of: {}".format(name))


# TODO(olegs): This and another x_from_dict can be rewritten in C.
def tuple_from_dict(ftype, factories, values):
    _, ttype, fmapping, _ = factories[ftype]
    args = [None] * (max(fmapping.values()) + 1)

    for k, v in values.items():
        args[fmapping[k]] = v

    return ttype(*args)


def enum_from_dict(ftype, factories, value):
    _, ttype, fmapping = factories[ftype]
    result = ttype(fmapping[value])
    return result


def extract_type_name(tname):
    i = len(tname)
    while i > 0:
        i -= 1
        if tname[i] in b'.:':
            return tname[:i].decode('utf-8'), tname[i + 1:].decode('utf-8')
    return '', tname.decode('utf-8')


def enum_desc(ftype, desc, factories, descriptions):
    ftype = ftype.replace(b':', b'.')
    if ftype in factories:
        return

    members = {}
    mmapping = {}

    for i, field in enumerate(desc):
        name = field[0].decode('utf-8')
        num = field[1]

        members[name] = i
        mmapping[num] = i

    result = IntEnum(ftype.decode('utf-8'), members)
    factories[ftype] = tuple([enum_from_dict, result, mmapping])


def message_desc(ftype, desc, factories, descriptions):
    # TODO(olegs): We need to handle package <-> message name
    # collision earlier
    ftype = ftype.replace(b':', b'.')
    if ftype in factories:
        return

    fields = {}
    unresolved = {}
    tmapping = {}
    fields_list = []
    fmapping = {}

    for field in desc:
        field_vt = field[0]
        field_name = field[2].decode('utf-8')
        field_type = field[1]
        field_num = field[3]
        if field_type not in _builtin_types:
            field_desc = factories.get(field_type, None)
            if not field_desc:
                unresolved[field_name] = field_type, field_num, field_vt

        if field_name not in fields:
            fields_list.append(field_name)
            fmapping[field_num] = len(fields_list) - 1
            fields[field_name] = len(fields_list) - 1
        else:
            fmapping[field_num] = fields[field_name]
        if field_vt == 7:
            tmapping[field_num] = field_type
        elif field_vt == 8:
            tmapping[field_num] = [field_type]
        else:
            raise Exception('Unrecognized field type: {}'.format(field_vt))

    module, name = extract_type_name(ftype)
    result = namedtuple(name, fields_list)
    result.__module__ = module
    factories[ftype] = result

    # TODO(olegs): I don't think these are possible.
    for field_name, (field_type, field_num, field_vt) in unresolved.items():
        if field_vt == 0:
            message_desc(
                field_type,
                find_desc(field_type, descriptions),
                factories,
                descriptions,
            )
        elif field_vt == 1:
            enum_desc(
                field_type,
                find_desc(field_type, descriptions),
                factories,
                descriptions,
            )

    factories[ftype] = tuple([tuple_from_dict, result, fmapping, tmapping])


def create_descriptors(descriptions):
    factories = {}

    print('descriptions: {}'.format(descriptions))
    for file, desc in descriptions.items():
        for d in desc:
            if d:
                rtype, tname, *fields = d
                if rtype == 0:
                    message_desc(
                        tname,
                        normalize_fields(fields),
                        factories,
                        descriptions,
                    )
                elif rtype == 1:
                    enum_desc(
                        tname,
                        fields,
                        factories,
                        descriptions,
                    )

    print('factories: {}'.format(factories))
    return factories
