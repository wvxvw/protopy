# -*- coding: utf-8 -*-

import pkg_resources

from os import path
from collections import namedtuple

from protopy.parser import BinParser
from protopy.serializer import Serializer


class TicTacToeGame:

    Score = namedtuple('Score', ['val', 'pos'])

    rows = {
        0: (1, 2),
        1: (0, 2),
        2: (0, 1),

        3: (4, 5),
        4: (3, 5),
        5: (3, 4),

        6: (7, 8),
        7: (6, 8),
        8: (6, 7),
    }

    cols = {
        0: (3, 6),
        3: (0, 6),
        6: (0, 3),

        1: (4, 7),
        4: (1, 7),
        7: (1, 4),

        2: (5, 8),
        5: (2, 8),
        8: (2, 5),
    }

    diag = {
        0: (4, 8),
        1: (),
        2: (4, 6),
        3: (),
        4: (0, 2, 6, 8),
        5: (),
        6: (4, 2),
        7: (),
        8: (4, 0),
    }

    def __init__(self, cell_ctor):
        self.cell_ctor = cell_ctor

    def can_lose_diag(self, i, board):
        score = 0
        for d in self.diag[i]:
            score += (board.cells[d].x or 0)
        return score

    def can_win_diag(self, i, board):
        score = 0
        for d in self.diag[i]:
            score += (board.cells[d].o or 0)
        return score

    def triple_price(self, i, board, triple):
        a, b = triple[i]
        ax, bx = board.cells[a].x, board.cells[b].x
        ao, bo = board.cells[a].o, board.cells[b].o
        au, bu = board.cells[a].unset, board.cells[b].unset

        if ax and bx:
            return 6
        if ao and bo:
            return 8
        if (ax or bx) and (au or bu):
            return 4
        if (ao or bo) and (au or bu):
            return 3
        return 0

    def row_price(self, i, board):
        return self.triple_price(i, board, self.rows)

    def col_price(self, i, board):
        return self.triple_price(i, board, self.cols)

    def score(self, i, cell, board):
        if not cell.unset:
            return self.Score(pos=i, val=-1)
        score = 0

        score += self.can_lose_diag(i, board) * 3
        score += self.can_win_diag(i, board)
        score += self.col_price(i, board)
        score += self.row_price(i, board)

        return self.Score(pos=i, val=score)

    def move(self, board):
        best = max(
            (self.score(i, c, board) for i, c in enumerate(board.cells)),
            key=lambda x: x.val,
        )
        board.cells[best.pos] = self.cell_ctor(x=None, o=True, unset=None)


class TicTacToeServer:

    def __init__(self):
        self.buffsize = 100
        self.sessions = {}
        root = pkg_resources.resource_filename(
            __name__,
            'etc',
        )
        self.parser = BinParser([root])
        self.proto_def = path.join(root, 'tictactoe.proto')
        self.parser.def_parser.parse(self.proto_def)
        self.serializer = Serializer(self.parser)
        self.connected = False
        self.dparser = self.parser.def_parser
        self.cell_ctor = self.dparser.find_definition('TicTacToeCell')
        self.game = TicTacToeGame(self.cell_ctor)

    async def turn(self, reader, writer):
        data, message = b'', None
        while True:
            try:
                try:
                    chunk = await reader.read(self.buffsize)
                    if not chunk:
                        self.connected = False
                        break
                    data += chunk
                except ConnectionResetError:
                    self.connected = False
                    writer.close()
                    return
                message = self.parser.parse(self.proto_def, 'TicTacToe', data)
            except Exception as e:
                print('what happened: {}'.format(e))
            else:
                break

        if self.connected:
            self.game.move(message.board)
            response = self.serializer.serialize(message, 'TicTacToe')
            writer.write(response)
            try:
                await writer.drain()
            except ConnectionResetError:
                self.connected = False
                writer.close()

    async def handle(self, reader, writer):
        self.connected = True
        while self.connected:
            await self.turn(reader, writer)


server = TicTacToeServer()


async def handler(reader, writer):
    await server.handle(reader, writer)
