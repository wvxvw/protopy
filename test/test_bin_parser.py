# -*- coding: utf-8 -*-
from protopy.parser import BinParser
from subprocess import Popen, PIPE

import pkg_resources
import asyncio


def test_gen_load_file():
    proto_roots = pkg_resources.resource_filename(
        __name__,
        './resources',
    )
    roots = [proto_roots]
    test_proto = pkg_resources.resource_filename(
        __name__,
        './resources/test.proto',
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
    stdout, stderr = protoc.communicate(
        input=b'test: 123\ntest_whatever: "123456"',
    )
    if protoc.returncode:
        print(stderr)
        assert False, stderr.decode('utf-8')

    print('generated proto message: {}'.format(stdout))
    loop = asyncio.get_event_loop()
    reader = asyncio.StreamReader(loop=loop)
    reader.feed_data(stdout)

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
