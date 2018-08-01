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
    for f, p in parser.files():
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


def test_describe_type():
    roots = [
        pkg_resources.resource_filename(__name__, './resources')
    ]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/deeply_nested_def.proto',
    )
    parser = DefParser(roots)
    parser.parse(test_proto)

    raw = {
        'a': 'foo',
        'b': 'bar',
        'j': 1234567,
        'k': True,

        'd': {
            'a': {
                'a': 2,
                'b': 'spam',
                'c': 'egg',
                'd': False,
            },
            'b': {
                'a': 0,
                'b': 'ham-bar',
                'c': 'bar-ham',
                'd': True,
            },
            'c': False,
        },
        'e': {
            'b': 1,
            'c': {
                'a': 'foo-spam',
                'b': 'spam-foo',
            },
        },
        'f': {
            'd': 1,
        },
        'g': {
            'b': 0,
            'c': {
                'a': -123456,
            },
        },
    }

    values = {}

    for k, v in parser.describe_type('TopLevel').items():
        print('k: {}, v: {}'.format(k, v))
        values[k] = v(raw.get(k, None))

    ptype = parser.find_definition('TopLevel')
    result = ptype(**values)
    print(result)
