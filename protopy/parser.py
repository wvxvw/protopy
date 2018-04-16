# -*- coding: utf-8 -*-

from protopy.wrapped import proto_def_parse


class DefParser:

    def __init__(self, roots):
        self.roots = [str(r).encode('utf-8') for r in roots]
        self.files = {}

    def parse(self, source):
        proto_def_parse(str(source).encode('utf-8'), self.roots, self.files)
