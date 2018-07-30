# -*- coding: utf-8 -*-

import pkg_resources
import asyncio

from os import path

from protopy.parser import BinParser
from protopy.serializer import Serializer


class TicTacToeClient:

    def __init__(self):
        root = pkg_resources.resource_filename(
            __name__,
            'etc',
        )
        self.buffsize = 100
        self.parser = BinParser([root])
        self.proto_def = path.join(root, 'tictactoe.proto')
        self.parser.def_parser.parse(self.proto_def)
        self.serializer = Serializer(self.parser)
        self.dparser = self.parser.def_parser
        self.message_ctor = self.dparser.find_definition('TicTacToe')
        self.board_ctor = self.dparser.find_definition('TicTacToeBoard')
        self.cell_ctor = self.dparser.find_definition('TicTacToeCell')
        self.create_board()

    def create_board(self):
        cells = [self.cell_ctor(x=None, o=None, unset=True) for _ in range(9)]
        self.board = self.board_ctor(cells=cells, session=0)

    def board_str(self):
        tpl = '\n---------\n'.join(['{} | {} | {}'] * 3)
        masks = {
            self.cell_ctor(x=True, o=None, unset=None): 'x',
            self.cell_ctor(x=None, o=True, unset=None): 'o',
            self.cell_ctor(x=None, o=None, unset=True): ' ',
        }
        vals = [masks[x] for x in self.board.cells]
        return tpl.format(*vals)

    def what_error(self, error):
        for e in error:
            if e:
                return e

    async def exchange(self):
        message = self.message_ctor(board=self.board, error=None)
        payload = self.serializer.serialize(message, 'TicTacToe')
        self.writer.write(payload)
        await self.writer.drain()

        data = b''
        while True:
            chunk = await self.reader.read(self.buffsize)
            data += chunk
            try:
                message = self.parser.parse(self.proto_def, 'TicTacToe', data)
            except Exception as e:
                print('what happened: {}'.format(e))
            else:
                break
        if message.error:
            raise Exception('protocol error: {}'.format(
                self.what_error(message.error),
            ))
        self.board = message.board

    async def connect(self, loop):
        reader, writer = await asyncio.open_connection(
            '127.0.0.1',
            8888,
            loop=loop,
        )
        self.reader = reader
        self.writer = writer
        await self.exchange()
        print(self.board_str())

    async def move(self, x, y):
        self.board.cells[y * 3 + x] = self.cell_ctor(
            x=True,
            o=None,
            unset=None,
        )
        print(self.board_str())
        await self.exchange()
        print(self.board_str())


client = TicTacToeClient()


async def connect(loop):
    await client.connect(loop)


async def move(x, y):
    await client.move(x, y)
