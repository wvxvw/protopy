# -*- coding: utf-8 -*-

from collections import namedtuple
from enum import IntEnum
from keyword import kwlist

from protopy.wrapped import (
    proto_def_parse,
    proto_parse,
    make_state,
    state_ready,
    state_result,
    state_set_factory,
    make_apr_pool,
)

_PB_TYPES = {
    b'int32': 0,
    b'int64': 1,
    b'uint32': 2,
    b'uint64': 3,
    b'sint32': 4,
    b'sint64': 5,
    b'bool': 6,
    b'fixed64': 8,
    b'sfixed64': 9,
    b'double': 10,
    b'string': 11,
    b'bytes': 12,
    b'fixed32': 15,
    b'sfixed32': 16,
}


class DefParser:

    def __init__(
            self,
            roots,
            enum_ctor=IntEnum,
            message_ctor=namedtuple,
            mp=None,
    ):
        self.enum_ctor = enum_ctor
        self.message_ctor = message_ctor
        self.roots = list(set([str(r).encode('utf-8') for r in roots]))
        self.files = {}
        self.defs = {}
        if mp:
            self.mp = mp
        else:
            self.mp = make_apr_pool()

    def parse(self, source, force=False):
        source = str(source).encode('utf-8')
        if source not in self.files or force:
            self.defs.update(
                proto_def_parse(
                    source,
                    self.roots,
                    self.files,
                    self.message_ctor,
                    self.enum_ctor,
                    kwlist,
                    self.mp,
                ),
            )


class BinParser:

    def __init__(self, roots):
        self.mp = make_apr_pool()
        self.def_parser = DefParser(roots, mp=self.mp)
        self.state = None

    def parse(self, source, message, buf):
        self.def_parser.parse(source)

        if not isinstance(message, bytes):
            message = str(message).encode('utf-8')
        self.state = make_state()
        state_set_factory(
            self.state,
            message,
            self.def_parser.defs,
            _PB_TYPES,
        )
        proto_parse(buf, self.state)

        if not state_ready(self.state):
            raise RuntimeError('Parser did not finish')
        return state_result(self.state)
