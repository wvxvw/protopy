# -*- coding: utf-8 -*-
from protopy.parser import DefParser

import pkg_resources


def test_load_file():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/test.proto',
    )
    print(DefParser(roots).parse(test_proto))


def test_nonexisting_import():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/nonexistent_import.proto',
    )
    try:
        DefParser(roots).parse(test_proto)
    except FileNotFoundError as e:
        assert 'nonexistent.proto' in str(e)


def test_imported_oneof():
    roots = [
        pkg_resources.resource_filename(__name__, './'),
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/test_imported_oneof.proto',
    )
    parser = DefParser(roots)
    parser.parse(test_proto)

    fields = []
    for f, p in parser.files.items():
        if b'test_imported_oneof.proto' in f:
            for r in p:
                if p[0] == 0:   # message
                    fields += p[2:]
    assert all(
        field[1].startswith(b'resources.')
        for field in fields
        if not type(field[1]) is list
    )


def test_iterate_defs():
    roots = [
        pkg_resources.resource_filename(__name__, './resources'),
        pkg_resources.resource_filename(__name__, '.')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/test_imported_oneof.proto',
    )
    parser = DefParser(roots)
    parser.parse(test_proto)

    passed = False
    for k, v in parser.definitions():
        if k == b'resources.Test.EmbeddedMessage':
            passed = True

    assert passed
