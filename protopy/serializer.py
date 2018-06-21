# -*- coding: utf-8 -*-
from protopy.wrapped import proto_serialize
from protopy.parser import ensure_bytes


class Serializer:
    def __init__(self, parser):
        self.parser = parser

    def serialize(self, message, ptype):
        ptype = ensure_bytes(ptype)
        return proto_serialize(message, ptype, self.parser.def_parser.defs)
