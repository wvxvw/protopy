# -*- coding: utf-8 -*-

import asyncio

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

    def __init__(self, roots, loop=None):
        if not loop:
            self.loop = asyncio.get_event_loop()
        self.def_parser = DefParser(roots)
        self.state = None

    def clone(self):
        result = BinParser(self.roots, self.loop)
        result.def_parser = self.def_parser
        return result

    async def parse_chunk(self, reader):
        chunk = await reader.read()
        proto_parse(chunk, self.state)

    async def parse(self, source, message, reader):
        self.def_parser.parse(source)
        if not isinstance(message, bytes):
            message = str(message).encode('utf-8')
        self.state = make_state()
        state_set_factory(
            self.state,
            message,
            # TODO(olegs): These two are the same, why do I need both of them?
            self.def_parser.defs,
            self.def_parser.defs,
        )
        result = None

        try:
            while not state_ready(self.state):
                await self.parse_chunk(reader)
            result = state_result(self.state)
        finally:
            self.state = None
        return result
