# -*- coding: utf-8 -*-

from collections import namedtuple
from enum import IntEnum
from keyword import kwlist
from os import path

from protopy.wrapped import (
    proto_def_parse,
    proto_parse,
    make_state,
    state_ready,
    make_apr_pool,
    apr_hash_find,
    apr_hash_replace,
    apr_update_hash,
    make_apr_hash,
    apr_hash_iterator,
    apr_hash_get_kv,
)


def ensure_bytes(s):
    if isinstance(s, bytes):
        return s
    return str(s).encode('utf-8')


def simple_enum(name, fields):
    inverse = {v: '{}.{}'.format(name, k) for k, v in fields.items()}

    def ctor(n):
        return inverse[n]

    return ctor


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

    def find_definition(self, definition):
        return apr_hash_find(self.defs, ensure_bytes(definition))

    def update_definition(self, definition, new):
        apr_hash_replace(self.defs, ensure_bytes(definition), new)

    def parse(self, source, force=False):
        source = ensure_bytes(source)
        if not force:
            if source in self.files:
                return
            for r in self.roots:
                if path.join(r, source) in self.files:
                    return

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
        result = proto_parse(buf, self.state)

        if not state_ready(self.state):
            raise RuntimeError('Parser did not finish')
        return result
