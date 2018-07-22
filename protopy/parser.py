# -*- coding: utf-8 -*-
'''
Python interface to Protobuf IML and binary parsing code.
'''

from collections import namedtuple
from enum import IntEnum
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
    '''
    Helper procedure to convert ``s`` to ``bytes`` if necessary.
    '''
    if isinstance(s, bytes):
        return s
    return str(s).encode('utf-8')


def simple_enum(name, fields):
    '''
    Python's built-in ``enum.IntEnum`` class, which is used by the
    parser by default may be too expensive memory and speed-wise.

    This procedure provides a light-weight alternative, which parse
    enumerator members as strings with the name of that member.

    Note that this procedure cannot be used if you also want to
    serialize enumerator values later.

    :param name: The name of the type of the enumerator you will be
        creating.
    :param fields: A dictionary mapping enumerator field names to
        the numerical values as defined in Protobuf IML.
    '''
    inverse = {v: '{}.{}'.format(name, k) for k, v in fields.items()}

    def ctor(n):
        return inverse[n]

    return ctor


class DefParser:
    '''
    Provides Python interface to IML parsing C code.
    '''
    def __init__(
            self,
            roots,
            enum_ctor=IntEnum,
            message_ctor=namedtuple,
            mp=None,
    ):
        '''
        Creates new Protobuf IML parser.  The parser will resolve source
        relative to ``roots`` directories.  This is equivalent to
        ``protoc -I`` option.

        :param roots: Directories relative to which the parser should try
            to find sources and their dependencies.

        :param enum_ctor: A callable to use as a factory for creating
            enumerator classes.
        :param message_ctor: A callable to use as a factory for creating
            message classes.
        :param mp: A capsule containing a pointer to APR memory pool this
            parser will use to allocate memory.  Note that this parser
            has internal state that relies on this pool to exist as
            long as the state needs to be accessed.  I.e. the
            definitions aren't actually Python objects, and are stored
            in this pool.  Do not try to free it unless you also
            delete the parser.

        It is not generally a good idea to pass this argument explicitly
        unless you know what you are doing.
        '''
        self.enum_ctor = enum_ctor
        self.message_ctor = message_ctor
        self.roots = list(set([ensure_bytes(r) for r in roots]))
        if mp:
            self.mp = mp
        else:
            self.mp = make_apr_pool()
        self._files = make_apr_hash(self.mp)
        self._defs = make_apr_hash(self.mp)

    def definitions(self):
        '''
        Returns an iterator that looks at every definition this parser has
        parsed so far.

        Example:

            .. code-block:: python

               for k, v in parser.definitions():
                   if k == b'foo.bar.Baz':
                       print('Baz found!')
        '''
        it = apr_hash_iterator(self._defs)
        while it:
            k, v = apr_hash_get_kv(it, 0)
            if k is not None:
                yield k, v
            else:
                break

    def files(self):
        '''
        Returns an iterator that looks at every parsed file and returns
        definitions found in that file.  The result is a tuple of
        messages and enums.

        Example:

            .. code-block:: python

               for k, v in parser.files():
                   if k == b'foo/bar/Baz.proto':
                       print('Baz found!')
        '''
        it = apr_hash_iterator(self._files)
        while it:
            k, v = apr_hash_get_kv(it, 1)
            if k is not None:
                yield k, v
            else:
                break

    def find_definition(self, definition):
        '''
        Returns a factory used to generate Python objects associated with
        ``definition`` (the fully qualified name of the Protobuf type
        as it appears in IDL).

        :param definition: The name of Protobuf type.
        '''
        return apr_hash_find(self._defs, ensure_bytes(definition), 0)

    def find_file(self, file):
        '''
        Returns definitions declared in ``file``.

        :param file: The file already parsed by this parser.
        '''
        return apr_hash_find(self._files, ensure_bytes(file), 1)

    def update_definition(self, definition, new):
        '''
        Replaces the factory associated with ``definition`` with ``new``
        factory.

        Note that you cannot replace factories for built-in Protobuf
        types (eg. ``int32`` or ``string``).

        This may be useful when you need to convert or fix messages
        before they are passed into constructors of the messages they
        belong to.

        ``new`` must be a callable that takes the same arguments as the
        replaced callable did.

        Example:

            .. code-block:: python

               original = parser.find_definition(b'Wrapper')

               def replacement(*args):
                   return {'replaced': original(*args)}
               parser.update_definition(b'Wrapper', replacement)
        '''
        apr_hash_replace(self._defs, ensure_bytes(definition), new)

    def parse(self, source, force=False):
        '''
        Parses ``source`` file (which will be looked up relative to the
        ``roots`` of this parser) together with all its dependencies.

        :param source: The Protobuf IML file to parse.
        :param force: If ``True`` forces re-parsing of the ``source``,
            even if it is already in the parser's cache.
        '''
        source = ensure_bytes(source)
        if not force:
            if apr_hash_find(self._files, source, 1):
                return
            for r in self.roots:
                if apr_hash_find(self._files, path.join(r, source), 1):
                    return
                rpath = path.relpath(source, r)
                if apr_hash_find(self._files, rpath, 1):
                    return

        self.parse_many([source])

    def parse_many(self, sources):
        apr_update_hash(
            self._defs,
            proto_def_parse(
                [ensure_bytes(s) for s in sources],
                self.roots,
                self._files,
                self.message_ctor,
                self.enum_ctor,
                self.mp,
            ),
        )


class BinParser:
    '''
    Parses binary Protobuf format.
    '''

    def __init__(self, roots):
        '''
        :param roots: list of directories relative to which the ``DefParser``
        created by this parser will try to resolve file names needed to
        parse the Protobuf definitions.
        '''
        self.mp = make_apr_pool()
        self.def_parser = DefParser(roots, mp=self.mp)
        self.state = None

    def parse(self, source, message, buf):
        '''
        Parses ``buf`` (binary content) using ``message`` as the name for
        the schema.  The shcema is read from ``source`` file.

        :param source: The file where schema (definition) for the message
            is to be found.
        :param message: The message to be parsed.  Unlike serializer, this
            does not allow parsing simple (not container) types.
        :param buf: The binary payload to parse.
        '''
        self.def_parser.parse(source)

        if not isinstance(message, bytes):
            message = str(message).encode('utf-8')
        self.state = make_state(
            message,
            self.def_parser._defs,
            self.mp,
        )
        result = proto_parse(buf, self.state)

        if not state_ready(self.state):
            raise RuntimeError('Parser did not finish')
        return result
