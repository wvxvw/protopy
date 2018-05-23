# -*- coding: utf-8 -*-

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
        source = str(source).encode('utf-8')
        if source not in self.files or force:
            self.defs.update(
                proto_def_parse(source, self.roots, self.files),
            )
            print('files updated for: {} not in {}'.format(source, self.files))


class BinParser:

    def __init__(self, roots):
        self.def_parser = DefParser(roots)
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
        )

        proto_parse(buf, self.state)

        if not state_ready(self.state):
            raise RuntimeError('Parser did not finish')
        return state_result(self.state)
