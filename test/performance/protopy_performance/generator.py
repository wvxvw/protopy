#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

import sys

from random import Random
from string import Template
from mnemonicode import mnformat
from hashlib import md5
from itertools import chain
from argparse import ArgumentParser


class Node:

    keytypes = [
        'int32', 'uint32', 'sint32', 'int64', 'uint64', 'sint64',
        'fixed32', 'sfixed32', 'fixed64', 'sfixed64',
        'string', 'bool',
    ]

    builtins = keytypes + ['bytes', 'double']

    def __init__(self, ctx, ratio, tpl, indent):
        if tpl:
            tpl = tpl.strip().split('\n')
            tpl = '\n'.join(t.strip() for t in tpl)
            self.tpl = Template(tpl)
        else:
            self.tpl = None
        self.ctx = ctx
        self.ratio = ratio
        self.indent = indent
        self.vars = {}
        self.ratio = ratio
        self.name_base = self.ratio.r.randint(0, sys.maxsize)

    def render(self):
        return self.tpl.substitute(self.vars)

    def pad(self, s):
        return '\n'.join('    ' + line for line in s.split('\n'))

    def __str__(self):
        return self.render()

    def next_name(self):
        self.name_base += 1
        m = md5()
        n = self.name_base
        b = []
        while n > 0:
            b.append(n & 0xFF)
            n >>= 8
        m.update(bytes(b))
        x = self.ratio.r.randint(3, 6)
        return mnformat(m.digest()[:x])

    def rtname_create(self, ctx):
        raw = self.next_name()
        n = ''.join(r.capitalize() for r in raw.split('-') if r)
        return ctx.put(n)

    def rtname_get(self, ctx):
        return self.ratio.r.choice(list(ctx.in_scope()))

    def rmember(self):
        raw = self.next_name()
        return '_'.join(r.upper() for r in raw.split('-') if r)

    def rident(self):
        raw = self.next_name()
        return '_'.join(r for r in raw.split('-') if r)

    def rint(self):
        return self.ratio.r.randint(
            self.ratio.previous + 1,
            self.ratio.previous + 100,
        )

    def rkeytype(self):
        return self.ratio.r.choice(self.keytypes)

    def builtin_type(self):
        return self.ratio.r.choice(self.builtins)


class FieldNode(Node):

    template = '$repeated$tname $fname = $n;'

    def __init__(self, ctx, ratio, indent, repeated):
        super().__init__(ctx, ratio, FieldNode.template, indent)
        self.repeated = repeated
        self.tname = self.rtname_get(self.ctx)
        self.fname = self.rident()
        self.n = self.rint()
        self.vars['repeated'] = self.repeated
        self.vars['tname'] = self.tname
        self.vars['fname'] = self.fname
        self.vars['n'] = self.n


class MemberNode(Node):

    template = '$name = $n;'

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, MemberNode.template, indent)
        self.name = self.rmember()
        self.n = self.rint()
        self.vars['name'] = self.name
        self.vars['n'] = self.n


class RepeatedNode(FieldNode):

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, indent, 'repeated ')


class SimpleNode(FieldNode):

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, indent, '')


class BuiltinNode(FieldNode):

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, indent, '')
        self.vars['tname'] = self.builtin_type()


class MapNode(FieldNode):

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, indent, 'map ')
        tname = self.vars['tname']
        self.vars['tname'] = '<{}, {}>'.format(self.rkeytype(), tname)


class OneOfFieldsNode(Node):

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, None, indent)

    def render(self):
        nodes = [
            d(self.ctx, self.ratio, self.indent + 1)
            for d in self.ratio.gen_oneof_fields(self.ctx, self.indent + 1)
        ]
        return '\n'.join(self.pad(n.render()) for n in nodes)


class OneofNode(Node):

    template = '''
    oneof $name {
    $fields
    }
    '''

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, OneofNode.template, indent)
        self.vars['name'] = self.rident()
        self.vars['fields'] = OneOfFieldsNode(
            self.ctx,
            self.ratio,
            self.indent,
        )

    def render(self):
        if self.ratio.can_recur(self.indent + 1):
            return super().render()
        return ''


class FieldsNode(Node):

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, None, indent)

    def render(self):
        nodes = [
            d(self.ctx, self.ratio, self.indent + 1)
            for d in self.ratio.gen_fields(self.ctx, self.indent + 1)
        ]
        return '\n'.join(self.pad(n.render()) for n in nodes)


class MembersNode(Node):

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, None, indent)

    def render(self):
        nodes = [MemberNode(self.ctx, self.ratio, self.indent + 1)]
        nodes += [
            MemberNode(self.ctx, self.ratio, self.indent + 1)
            for _ in self.ratio.members(self.indent + 1)
        ]
        return '\n'.join(self.pad(n.render()) for n in nodes)


class DefsNode(Node):

    def __init__(self, ctx, ratio, indent, toplevel=False):
        super().__init__(ctx, ratio, None, indent)
        self.toplevel = toplevel

    def render(self):
        nodes = [
            d(self.ctx, self.ratio, self.indent + 1)
            for d in self.ratio.gen_defs(self.ctx, self.indent + 1)
        ]
        if self.toplevel:
            return '\n'.join(n.render() for n in nodes)
        return '\n'.join(self.pad(n.render()) for n in nodes)


class MessageNode(Node):

    template = '''
    message $name {
    $defs
    $fields
    }
    '''

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, MessageNode.template, indent)
        self.ratio.previous = 0
        self.name = self.rtname_create(self.ctx)
        child_ctx = TypeContext(self.ctx)
        child_ctx.name = self.name
        self.fields = FieldsNode(child_ctx, self.ratio, self.indent)
        self.defs = DefsNode(child_ctx, self.ratio, self.indent)
        self.vars['name'] = self.name
        self.vars['fields'] = self.fields
        self.vars['defs'] = self.defs

    def render(self):
        if self.ratio.can_recur(self.indent + 1):
            return super().render()
        return ''


class EnumNode(Node):

    template = '''
    enum $name {
    $members
    }
    '''

    def __init__(self, ctx, ratio, indent):
        super().__init__(ctx, ratio, EnumNode.template, indent)
        self.ratio.previous = 0
        self.name = self.rtname_create(ctx)
        self.members = MembersNode(self.ctx, self.ratio, self.indent)
        self.vars['name'] = self.name
        self.vars['members'] = self.members


class Ratio:

    def __init__(self, seed):
        self.seed = seed
        self.r = Random()
        self.r.seed(seed)
        distributions = [self.r.random() for _ in range(2)]
        isum = 1.0 / sum(distributions)
        distributions = [isum * x for x in distributions]
        self.messages, self.enums = distributions

        field_types = [self.r.random() for _ in range(5)]
        isum = 1.0 / sum(field_types)
        field_types = [isum * x for x in field_types]
        self.sfields, self.mfields, self.repeated, \
            self.maps, self.oneofs = field_types

        self.depth = self.r.uniform(0.1, 0.3)
        self.breadth = self.r.uniform(0.1, 0.7)
        self.previous = 0

    def __str__(self):
        s = '''
        /**
         * seed:               {}
         * ---------------------------------------
         * p(messages):        {}
         * p(enums):           {}
         * p(built-in fields): {}
         * p(message fields):  {}
         * p(repeated fields): {}
         * p(oneof fields):    {}
         * p(map fields):      {}
         * ---------------------------------------
         * p(depth):           {}
         * p(breadth):         {}
         */
        '''.format(
            self.seed,
            self.messages,
            self.enums,
            self.sfields,
            self.mfields,
            self.repeated,
            self.oneofs,
            self.maps,
            self.depth,
            self.breadth,
        )
        return '\n'.join(l.strip() for l in s.split('\n'))

    def def_type(self):
        total = self.messages + self.enums
        coeff = 1.0 / total
        mfrac = self.messages * coeff
        if self.r.random() < mfrac:
            return MessageNode
        return EnumNode

    def field_type(self):
        r = self.r.random()
        t = 0
        dist = (
            self.sfields, self.mfields, self.repeated,
            self.oneofs, self.maps,
        )
        types = (
            BuiltinNode, SimpleNode, RepeatedNode, OneofNode, MapNode,
        )

        for d, ft in zip(dist, types):
            if t + d > r:
                return ft
            t += d

        return MapNode

    def oneof_field_type(self):
        r = self.r.random()
        total = self.sfields + self.mfields
        coeff = 1 / total
        dist = self.sfields * coeff, self.mfields * coeff

        if r < dist[0]:
            return BuiltinNode
        return SimpleNode

    def can_recur(self, indent):
        prob = ((10 - indent) * (10 - indent)) / 100.0
        return self.depth < prob

    def gen_defs(self, ctx, indent):
        for _ in self.members(indent):
            yield self.def_type()

    def gen_fields(self, ctx, indent):
        for _ in self.members(indent):
            yield self.field_type()

    def gen_oneof_fields(self, ctx, indent):
        for _ in self.members(indent):
            yield self.oneof_field_type()

    def members(self, indent):
        prob = (10 - indent) / 100.0
        return range(int((self.breadth * prob) / 0.01))


class TypeContext:

    def __init__(self, ctx=None):
        self.parent = ctx
        self.defs = []
        self.name = ''

    def in_scope(self):
        if self.parent:
            return chain(self.format_defs(), self.parent.in_scope())
        return self.defs

    def format_defs(self):
        for d in self.defs:
            yield '.'.join(self.prefix() + [d])

    def put(self, d):
        self.defs.append(d)
        return d

    def prefix(self):
        if self.parent:
            return self.parent.prefix() + [self.name]
        return [self.name]


class ProtoGenerator:

    header = 'syntax = "proto3";'

    def __init__(self, seed=None):
        self.ratio = Ratio(seed)
        self.ctx = TypeContext()

    def render(self):
        defs = DefsNode(self.ctx, self.ratio, 0, toplevel=True)
        return '\n\n'.join(map(str, [self.ratio, self.header, defs.render()]))


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument(
        '-s', '--seed',
        type=int,
        help='Seed to use when generating Proto file',
        required=True,
    )
    args = parser.parse_args()
    generator = ProtoGenerator(args.seed)
    for line in generator.render().split('\n'):
        if line.strip():
            print(line)
