# -*- coding: utf-8 -*-

from os import path
from time import time

from protopy.parser import BinParser


class Runner:

    def __init__(self, idl, bins, times=100, threads=1):
        self.idl = idl
        self.bins = bins
        self.threads = threads
        self.times = times

        self.protopy_parser = BinParser([path.dirname(self.idl)])

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
