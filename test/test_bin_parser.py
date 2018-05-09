# -*- coding: utf-8 -*-
from protopy.parser import BinParser
from subprocess import Popen, PIPE

import pkg_resources
import asyncio
import os

import pytest


def generate_proto_binary(source, text):
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
        '--encode=Test',
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


def test_gen_load_file():
    roots, test_proto, content = generate_proto_binary(
        'test.proto',
        b'test: 123\ntest_whatever: "123456"',
    )
    print('generated proto message: {}'.format(content))
    loop = asyncio.get_event_loop()
    reader = asyncio.StreamReader(loop=loop)
    reader.feed_data(content)

    async def finish():
        asyncio.sleep(2)
        reader.feed_eof()

    async def gather_results():
        parse_bin = BinParser(roots).parse(test_proto, 'Test', reader)
        return await asyncio.gather(parse_bin, finish())

    result = loop.run_until_complete(gather_results())[0]
    print('result: {}'.format(result))
    assert result.test == 123
    assert False


def test_inner_message():
    roots, test_proto, content = generate_proto_binary(
        'test_embedded.proto',
        b'''test: 123
            first_inner: {test_fixed: 4567}
            second_inner: {test_long: 135, test_short: 689}
        ''',
    )
    print('generated proto message: {}'.format(content))
    loop = asyncio.get_event_loop()
    reader = asyncio.StreamReader(loop=loop)
    reader.feed_data(content)

    async def finish():
        asyncio.sleep(2)
        reader.feed_eof()

    async def gather_results():
        parse_bin = BinParser(roots).parse(test_proto, 'Test', reader)
        return await asyncio.gather(parse_bin, finish())

    result = loop.run_until_complete(gather_results())[0]
    print('result: {}'.format(result))
    assert result.test == 123
    assert False
