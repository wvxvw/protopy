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
