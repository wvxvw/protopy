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
    result1 = parser.parse(test_proto, 'Test', content)

    serializer = Serializer(parser)

    result2 = parser.parse(
        test_proto,
        'Test',
        serializer.serialize(result1, 'Test'),
    )

    assert result1 == result2


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


def test_basic_types():
    roots, test_proto, content = generate_proto_binary(
        'simple_types.proto',
        b'''
        tint32: 123456
        tint64: 123456789123456789
        tsint32: -123456
        tsint64: -123456789123456789
        tuint32: 123456
        tuint64: 123456789123456789
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
    result = parser.parse(test_proto, 'SimpleTypes', content)
    print('result: {}'.format(result))
    serializer = Serializer(parser)

    assert serializer.serialize(result.tint32, 'int32') \
        == b'\xc0\xc4\x07'
    assert serializer.serialize(result.tint64, 'int64') \
        == b'\x95\xbe\xc1\xe6\xba\xe9\xa6\xdb\x01'
    assert serializer.serialize(result.tsint32, 'sint32') \
        == b'\xff\x88\x0f'
    assert serializer.serialize(result.tsint64, 'sint64') \
        == b'\xa9\xfc\x82\xcd\xf5\xd2\xcd\xb6\x03'
    assert serializer.serialize(result.tuint32, 'uint32') \
        == b'\xc0\xc4\x07'
    assert serializer.serialize(result.tuint64, 'uint64') \
        == b'\x95\xbe\xc1\xe6\xba\xe9\xa6\xdb\x01'
    assert serializer.serialize(result.tbool, 'bool') \
        == b'\x01'
    assert serializer.serialize(result.tfixed64, 'fixed64') \
        == b'\x15_\xd0\xacK\x9b\xb6\x01'
    assert serializer.serialize(result.tsfixed64, 'sfixed64') \
        == b'\xeb\xa0/S\xb4dI\xfe'
    assert serializer.serialize(result.tfixed32, 'fixed32') \
        == b'@\xe2\x01\x00'
    assert serializer.serialize(result.tsfixed32, 'sfixed32') \
        == b'\xc0\x1d\xfe\xff'
    assert serializer.serialize(result.tstring, 'string') \
        == b'\x07abcdefg'
    assert serializer.serialize(result.tbytes, 'bytes') \
        == b'\x03\x01\x02\x03'
    print('result.tdouble: {}'.format(result.tdouble))
    assert serializer.serialize(result.tdouble, 'double') \
        == b'\xa8\xff\xac\xf9\x01$\xfe@'
