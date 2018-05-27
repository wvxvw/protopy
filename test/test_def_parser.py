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


def test_parse_descriptor():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/google/protobuf/descriptor.proto',
    )
    print(DefParser(roots).parse(test_proto))


def test_internal_def():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/np/v1/upload-np-data-service.proto',
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
    print(parser.files)
    assert False
