# -*- coding: utf-8 -*-

import asyncio
from timeit import timeit

from protopy.wrapped import (
    proto_def_parse,
    proto_parse,
    make_state,
    state_ready,
    state_result,
    state_set_factory,
)


class DefParser:

    def __init__(self, roots):
        self.roots = list(set([str(r).encode('utf-8') for r in roots]))
        self.files = {}
        self.defs = {}

    def parse(self, source, force=False):
        if source not in self.files or force:
            self.defs.update(
                proto_def_parse(
                    str(source).encode('utf-8'),
                    self.roots,
                    self.files,
                ),
            )


class BinParser:

    def __init__(self, roots, loop=None, diagnostics=False):
        if not loop:
            self.loop = asyncio.get_event_loop()
        self.def_parser = DefParser(roots)
        self.state = None
        self.diagnostics = diagnostics

    async def parse_chunk(self, reader):
        chunk = await reader.read()
        proto_parse(chunk, self.state)

    async def parse_chunk_timed(self, reader):
        chunk = await reader.read()
        print('proto_parse #{} bytes {} seconds'.format(
            len(chunk),
            timeit(lambda: proto_parse(chunk, self.state), number=1),
        ))

    async def parse(self, source, message, reader):
        if self.diagnostics:
            print('parsing extra defs: {} seconds'.format(
                timeit(lambda: self.def_parser.parse(source), number=1),
            ))
        else:
            self.def_parser.parse(source)

        if not isinstance(message, bytes):
            message = str(message).encode('utf-8')
        self.state = make_state()
        state_set_factory(
            self.state,
            message,
            self.def_parser.defs,
        )
        result = None

        if self.diagnostics:
            while not state_ready(self.state):
                await self.parse_chunk_timed(reader)
        else:
            while not state_ready(self.state):
                await self.parse_chunk(reader)
        return state_result(self.state)
