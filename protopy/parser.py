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
    make_apr_pool,
    apr_hash_contains,
    apr_update_hash,
    make_apr_hash,
    apr_hash_iterator,
    apr_hash_get_kv,
)


def ensure_bytes(s):
    if isinstance(s, bytes):
        return s
    return str(s).encode('utf-8')


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
        self.roots = list(set([ensure_bytes(r) for r in roots]))
        self.files = {}
        if mp:
            self.mp = mp
        else:
            self.mp = make_apr_pool()
        self.defs = make_apr_hash(self.mp)

    def definitions(self):
        it = apr_hash_iterator(self.defs)
        while True:
            k, v = apr_hash_get_kv(it)
            if k is not None:
                yield k, v

    def has_definition(self, definition):
        return apr_hash_contains(self.defs, str(definition).encode('utf-8'))

    def parse(self, source, force=False):
        source = str(source).encode('utf-8')
        if source not in self.files or force:
            apr_update_hash(
                self.defs,
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
        self.state = make_state(
            message,
            self.def_parser.defs,
            self.mp,
        )
        proto_parse(buf, self.state)

        if not state_ready(self.state):
            raise RuntimeError('Parser did not finish')
        return state_result(self.state)
