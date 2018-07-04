# -*- coding: utf-8 -*-
'''
Python interface to Protobuf serializatin code.
'''

import mmap

from protopy.wrapped import proto_serialize
from protopy.parser import ensure_bytes


class Serializer:
    '''
    Instances of this class are tied to the parser.  In order to
    serialize anything, the parser must already have parsed the
    definitions you want to use during serialization.

    If you aren't sure if the definition was already parsed, you can
    call:

        .. code-block:: python

           serializer.parser.def_parser.parse(...)
    '''
    def __init__(self, parser):
        '''
        Creates new ``Serializer``.
        :param parser: An instance of ``protopy.parser.BinParser``.
        '''
        self.parser = parser

    def serialize(self, message, ptype):
        '''
        Serializes ``message`` (an arbitrary Python object) into Protobuf
        format, using ``ptype`` as a blueprint to extract field type
        information.  Unlike flagship implementation, it is not
        necessary for ``message`` to map to a message definition.  It
        can be an integer, a string etc.  For instance:

            .. code-block:: python

               serializer.serialize(42, b'int32')

        Will serialize 42 using varint encoding.

        Python objects which map to ``message`` definiton in Protobuf
        IML must implement `mapping protocol
        <https://docs.python.org/3/c-api/mapping.html>`_.  ``dic`` and
        objects inheriting this class implement this protocol, for
        example.

        Python objects which map to ``repeated`` fields in Protobuf
        IML must implement `sequence protocol
        <https://docs.python.org/3/c-api/sequence.html>`_.  ``list``,
        ``tuple``, ``str`` and many other built-in classes implement
        this protocol, for example.
        '''
        ptype = ensure_bytes(ptype)
        return proto_serialize(
            message,
            ptype,
            self.parser.def_parser.defs,
            mmap.PAGESIZE,
        )
