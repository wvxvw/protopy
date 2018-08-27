# -*- coding: utf-8 -*-

from subprocess import Popen, PIPE

import os
import pkg_resources


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
