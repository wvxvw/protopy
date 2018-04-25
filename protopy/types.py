# -*- coding: utf-8 -*-

from collections import namedtuple


def find_desc(dtype, name, descriptions):
    for ftype, fname, fields in descriptions:
        if ftype == dtype and fname == name:
            return fields
    # TODO(olegs): Consider lazy loading of proto descriptions.
    raise ValueError("Cannot find definition of: {}".format(name))


def message_factory(name, desc, factories, descriptions):
    fields = {}
    unresolved = {}
    for field in desc:
        field_name = field[1].decode('utf-8')
        field_type = field[2].decode('utf-8')
        field_desc = factories.get(field_type, None)
        if not field_desc:
            unresolved[field_name] = field_type, field[0]

        fields[field_name] = field_type, field[0]
    result = namedtuple(name, fields.keys())
    factories[name] = result
    for field_name, (field_type, field_num) in unresolved.items():
        field_desc = message_factory(
            field_name,
            find_desc(0, name, descriptions),
            factories,
            descriptions,
        )
