# -*- coding: utf-8 -*-

import sys

from os import path
from time import time
from importlib import import_module

from protopy.parser import BinParser


class Runner:

    def __init__(self, idl, pyidl, bins, times=100, threads=1):
        self.idl = idl
        self.pyidl = pyidl
        self.bins = bins
        self.threads = threads
        self.times = times

        self.protopy_parser = BinParser([path.dirname(self.idl)])
        self.protopy_parser.def_parser.parse(path.basename(self.idl))
        sys.path.insert(0, path.dirname(self.pyidl))
        self.pdef = import_module(
            path.splitext(path.basename(self.pyidl))[0]
        ).AllMessages

    def run_protopy(self):
        start, step = time(), 0
        io_time, parse_time = 0, 0
        size = 0

        for _ in range(self.times):
            for b in self.bins:
                with open(b, 'rb') as payload:
                    r = payload.read()
                    step = time() - start
                    io_time += step
                    start = step + start
                    size += len(r)
                    self.protopy_parser.parse(
                        path.basename(self.idl),
                        b'AllMessages',
                        r,
                    )
                    step = time() - start
                    parse_time += step
                    start = step + start

        print('Protopy:')
        print('========')
        print('Bytes processed: {}'.format(size))
        print('I/O time: {}'.format(io_time))
        print('Parse time: {}'.format(parse_time))

    def run_protobuf(self):
        start, step = time(), 0
        io_time, parse_time = 0, 0
        size = 0

        for _ in range(self.times):
            for b in self.bins:
                with open(b, 'rb') as payload:
                    r = payload.read()
                    step = time() - start
                    io_time += step
                    start = step + start
                    size += len(r)
                    d = self.pdef()
                    d.ParseFromString(r)
                    step = time() - start
                    parse_time += step
                    start = step + start

        print('Google Protobuf:')
        print('========')
        print('Bytes processed: {}'.format(size))
        print('I/O time: {}'.format(io_time))
        print('Parse time: {}'.format(parse_time))
