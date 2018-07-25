#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

from random import Random
from string import Template, ascii_lowercase, ascii_uppercase


class Node:

    lowercase = list(ascii_lowercase)
    uppercase = list(ascii_uppercase)
    idents = list(ascii_lowercase + '_')

    keytypes = [
        'int32', 'uin32', 'sint32', 'int64', 'uint64', 'sint64',
        'fixed32', 'sfixed32', 'fixed64', 'sfixed64',
        'string', 'bool',
    ]

    builtins = keytypes + ['bytes', 'double']

    def __init__(self, ratio, tpl, indent):
        tpl = tpl.strip().split('\n')
        tpl = '\n'.join(t.strip() for t in tpl)
        self.ratio = ratio
        self.tpl = Template(tpl)
        self.indent = indent
        self.vars = {}
        self.ratio = ratio

    def render(self):
        # print('processing template: {}, vars: {}'.format(
        #     self.tpl.template,
        #     self.vars,
        # ))
        return self.tpl.substitute(self.vars)

    def pad(self, s):
        return '\n'.join('    ' + line for line in s.split('\n'))

    def __str__(self):
        return self.render()

    def rtname(self):
        self.ratio.r.shuffle(self.lowercase)
        length = self.ratio.r.randint(1, len(self.lowercase))
        return ''.join(self.lowercase[:length]).capitalize()

    def rmember(self):
        self.ratio.r.shuffle(self.uppercase)
        length = self.ratio.r.randint(1, len(self.uppercase))
        return ''.join(self.uppercase[:length])

    def rident(self):
        self.ratio.r.shuffle(self.idents)
        length = self.ratio.r.randint(1, len(self.idents))
        return ''.join(self.idents[:length])

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

    def __init__(self, ratio, indent, repeated):
        super().__init__(
            ratio,
            FieldNode.template,
            indent,
        )
        self.repeated = repeated
        self.tname = self.rtname()
        self.fname = self.rident()
        self.n = self.rint()
        self.vars['repeated'] = self.repeated
        self.vars['tname'] = self.tname
        self.vars['fname'] = self.fname
        self.vars['n'] = self.n


class MemberNode(Node):

    template = '$name = $n;'

    def __init__(self, ratio, indent):
        super().__init__(
            ratio,
            MemberNode.template,
            indent,
        )
        self.name = self.rmember()
        self.n = self.rint()
        self.vars['name'] = self.name
        self.vars['n'] = self.n


class RepeatedNode(FieldNode):

    def __init__(self, ratio, indent):
        super().__init__(ratio, indent, 'repeated ')


class SimpleNode(FieldNode):

    def __init__(self, ratio, indent):
        super().__init__(ratio, indent, '')


class BuiltinNode(FieldNode):

    def __init__(self, ratio, indent):
        super().__init__(ratio, indent, '')
        self.vars['tname'] = self.builtin_type()


class MapNode(FieldNode):

    def __init__(self, ratio, indent):
        super().__init__(ratio, indent, 'map ')
        tname = self.vars['tname']
        self.vars['tname'] = '<{}, {}>'.format(self.rkeytype(), tname)


class FieldsNode(Node):

    def __init__(self, ratio, indent):
        self.ratio = ratio
        self.indent = indent

    def render(self):
        if self.ratio.can_recur(self.indent + 1):
            nodes = [
                d(self.ratio, self.indent + 1)
                for d in self.ratio.gen_fields(self.indent + 1)
            ]
            # print('rendering fields: {}'.format(nodes))
            return '\n'.join(self.pad(n.render()) for n in nodes)
        return ''


class MembersNode(Node):

    def __init__(self, ratio, indent):
        self.ratio = ratio
        self.indent = indent

    def render(self):
        nodes = [MemberNode(self.ratio, self.indent + 1)]
        nodes += [
            MemberNode(self.ratio, self.indent + 1)
            for _ in self.ratio.members(self.indent + 1)
        ]
        # print('rendering members: {}'.format(nodes))
        return '\n'.join(self.pad(n.render()) for n in nodes)


class DefsNode(Node):

    def __init__(self, ratio, indent, toplevel=False):
        self.ratio = ratio
        self.indent = indent
        self.toplevel = toplevel

    def render(self):
        if self.ratio.can_recur(self.indent + 1):
            nodes = [
                d(self.ratio, self.indent + 1)
                for d in self.ratio.gen_defs(self.indent + 1)
            ]
            # print('rendering defs: {}'.format(nodes))
            if self.toplevel:
                return '\n'.join(n.render() for n in nodes)
            return '\n'.join(self.pad(n.render()) for n in nodes)
        return ''


class MessageNode(Node):

    template = '''
    message $name {
    $defs
    $fields
    }
    '''

    def __init__(self, ratio, indent):
        super().__init__(
            ratio,
            MessageNode.template,
            indent,
        )
        self.ratio.previous = 0
        self.name = self.rtname()
        self.fields = FieldsNode(ratio, indent)
        self.defs = DefsNode(ratio, indent)
        self.vars['name'] = self.name
        self.vars['fields'] = self.fields
        self.vars['defs'] = self.defs


class EnumNode(Node):

    template = '''
    enum $name {
    $members
    }
    '''

    def __init__(self, ratio, indent):
        super().__init__(
            ratio,
            EnumNode.template,
            indent,
        )
        self.ratio.previous = 0
        self.name = self.rtname()
        self.members = MembersNode(ratio, indent)
        self.vars['name'] = self.name
        self.vars['members'] = self.members


class Ratio:

    def __init__(self, r):
        self.r = r
        distributions = [r.random() for _ in range(4)]
        isum = 1.0 / sum(distributions)
        distributions = [isum * x for x in distributions]
        self.messages, self.enums, self.fields, self.oneofs = distributions

        field_types = [r.random() for _ in range(4)]
        isum = 1.0 / sum(field_types)
        field_types = [isum * x for x in field_types]
        self.sfields, self.mfields, self.repeated, self.maps = field_types

        self.depth = r.uniform(0.1, 0.3)
        self.breadth = r.uniform(0.1, 0.7)
        self.previous = 0

    def def_type(self):
        total = self.messages + self.enums
        coeff = 1.0 / total
        mfrac = self.messages * coeff
        # print('chance of message: {}'.format(mfrac))
        if self.r.random() < mfrac:
            return MessageNode
        return EnumNode

    def field_type(self):
        r = self.r.random()
        t = 0
        dist = self.sfields, self.mfields, self.repeated, self.maps
        types = BuiltinNode, SimpleNode, RepeatedNode, MapNode

        for d, ft in zip(dist, types):
            if t + d > r:
                return ft
            t += d

        return MapNode

    def can_recur(self, indent):
        prob = ((10 - indent) * (10 - indent)) / 100.0
        print('can_recur: {} < {} / {} = {}'.format(
            self.depth,
            prob,
            indent,
            self.depth < prob,
        ))
        return self.depth < prob

    def gen_defs(self, indent):
        for _ in self.members(indent):
            yield self.def_type()

    def gen_fields(self, indent):
        for _ in self.members(indent):
            yield self.field_type()

    def members(self, indent):
        prob = (10 - indent) / 100.0
        print('prob: {}, generates {} members'.format(
            prob,
            indent,
            int((self.breadth * prob) / 0.002)),
        )
        return range(int((self.breadth * prob) / 0.01))


class ProtoGenerator:

    header = 'syntax = "proto3";'

    def __init__(self, seed=None):
        self.seed = seed
        self.r = Random()
        self.r.seed(seed)
        self.ratio = Ratio(self.r)

    def render(self):
        defs = DefsNode(self.ratio, 0, toplevel=True)
        return self.header + '\n\n' + defs.render()


if __name__ == '__main__':
    generator = ProtoGenerator()
    print(generator.render())
