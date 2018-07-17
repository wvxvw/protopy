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


# this one hangs
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

    found = False
    for f, p in parser.files.items():
        if b'test_imported_oneof.proto' in f:
            found = True
    assert found


# this one hangs too
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


def test_deep_nesting():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/deeply_nested_def.proto',
    )
    parser = DefParser(roots)
    parser.parse(test_proto)

    # 10 messages + 7 enums
    defcount = 10 + 7
    actual_defcount = 0
    for k, v in parser.definitions():
        print('def: {} => {}'.format(k, v))
        actual_defcount += 1

    assert defcount == actual_defcount


def test_nested_otpions():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/test_nested_options.proto',
    )
    parser = DefParser(roots)
    parser.parse(test_proto)


def test_rpc():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/test_rpc.proto',
    )
    parser = DefParser(roots)
    parser.parse(test_proto)


# this one hangs
def test_block_comment():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/test_block_comment.proto',
    )
    parser = DefParser(roots)
    parser.parse(test_proto)


def test_double_line_comment():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/test_double_line_comment.proto',
    )
    parser = DefParser(roots)
    parser.parse(test_proto)
