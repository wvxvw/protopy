# -*- mode: utf-8 -*-

from protopy.parser import DefParser
from protopy.wrapped import message_from_bytes

from support import generate_proto_binary


def test_basic_native_message():
    roots, test_proto, content = generate_proto_binary(
        'test.proto',
        b'test: 123\ntest_whatever: "123456"',
    )
    parser = DefParser(roots)
    parser.parse(test_proto)
    message = message_from_bytes(parser, b'Test', b'123')
    print('message: {}'.format(message.test_whatever))
