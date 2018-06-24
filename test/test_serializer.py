# -*- coding: utf-8 -*-
from protopy.parser import BinParser
from protopy.serializer import Serializer
from subprocess import Popen, PIPE

import pkg_resources
import os


def generate_proto_binary(source, text, message='Test'):
    proto_roots = pkg_resources.resource_filename(
        __name__,
        './resources',
    )
    roots = [proto_roots]
    test_proto = pkg_resources.resource_filename(
        __name__,
        os.path.join('./resources/', source),
    )
    cmd = [
        'protoc',
        '--encode={}'.format(message),
        '-I={}'.format(proto_roots),
        test_proto,
    ]
    print('calling protoc with: {}'.format(cmd))
    protoc = Popen(
        cmd,
        stdout=PIPE,
        stderr=PIPE,
        stdin=PIPE,
    )
    stdout, stderr = protoc.communicate(input=text)
    if protoc.returncode:
        raise Exception(stderr.decode('utf-8'))

    return roots, test_proto, stdout


def test_simple_serialize():
    roots, test_proto, content = generate_proto_binary(
        'test.proto',
        b'test: 123\ntest_whatever: "123456"',
    )
    print('generated proto message: {}'.format(content))

    parser = BinParser(roots)
    result = parser.parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.test == 123
    serializer = Serializer(parser)

    assert serializer.serialize(result, 'Test') == content


def test_integer():
    roots, test_proto, content = generate_proto_binary(
        'test.proto',
        b'test: 123\ntest_whatever: "123456"',
    )
    print('generated proto message: {}'.format(content))

    parser = BinParser(roots)
    result = parser.parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.test == 123
    serializer = Serializer(parser)

    assert serializer.serialize(result.test, 'int32') \
        == chr(123).encode('utf-8')

    assert serializer.serialize(123456, 'int32') == b'\xc0\xc4\x07'
    assert serializer.serialize(123456789, 'int32') == b'\x95\x9a\xef:'


def test_signed_integer():
    roots, test_proto, content = generate_proto_binary(
        'simple_types.proto',
        b'''
        tint32: 123456
        tint64: 123456789123456789
        tstint32: -123456
        tstint64: -123456789123456789
        tutint32: 123456
        tutint64: 123456789123456789
        tbool: true
        tfixed64: 123456789123456789
        tsfixed64: -123456789123456789
        tdouble: 123456.123456
        tstring: 'abcdefg'
        tbytes: '\x01\x02\x03'
        tfixed32: 123456
        tsfixed32: -123456
        ''',
        'SimpleTypes'
    )
    print('generated proto message: {}'.format(content))

    parser = BinParser(roots)
    result = parser.parse(test_proto, 'Test', content)
    serializer = Serializer(parser)

    assert serializer.serialize(result.tint32, 'int32') \
        == chr(123456).encode('utf-8')
