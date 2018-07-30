# -*- coding: utf-8 -*-

import pkg_resources
import sys
import traceback

from os import path
from collections import namedtuple

from protopy.parser import BinParser
from protopy.serializer import Serializer


def is_board_full(board):
    return all((x.x is not None or x.o is not None) for x in board.cells)


class TicTacToeError(Exception):

    def __init__(self, parser, message):
        super().__init__(message)
        self.parser = parser
        self.edef = parser.find_definition('TicTacToeError')

    ptypes = {
        'invalid_move': None,
        'board_full': None,
        'session_lost': None,
    }

    def proto(self):
        cpy = dict(self.ptypes)
        cpy[self.kind] = self.pctor(str(self))
        return self.edef(**cpy)


class BoardFull(TicTacToeError):

    def __init__(self, parser, message):
        super().__init__(parser, message)
        self.kind = 'board_full'
        self.pctor = parser.find_definition('BoardFull')


class InvalidMove(TicTacToeError):

    def __init__(self, parser, message):
        super().__init__(parser, message)
        self.kind = 'invalid_move'
        self.pctor = parser.find_definition('InvalidMove')


class SessionLost(TicTacToeError):

    def __init__(self, parser, message):
        super().__init__(parser, message)
        self.kind = 'session_lost'
        self.pctor = parser.find_definition('SessionLost')


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

    def validate(self, board, parser):
        if len(board.cells) != 9:
            raise InvalidMove(
                parser,
                'Board must have exactly 9 cells, {} given'.format(
                    len(board.cells),
                ))
        if is_board_full(board):
            raise BoardFull(
                parser,
                'Board is full',
            )

    def move(self, board, parser):
        self.validate(board, parser)
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
        self.game_ctor = self.dparser.find_definition('TicTacToe')
        self.board_ctor = self.dparser.find_definition('TicTacToeBoard')
        self.game = TicTacToeGame(self.cell_ctor)
        self.last_session = 0

    def gensession(self):
        self.last_session += 1
        return self.last_session

    def diff_boards(self, a, b):
        diff = {}
        for i, (ca, cb), in enumerate(zip(a.cells, b.cells)):
            if ca != cb:
                changes = []
                if ca.x != cb.x:
                    changes.append('x')
                if ca.o != cb.o:
                    changes.append('o')
                if ca.unset != cb.unset:
                    changes.append('unset')
                diff[i] = changes
        return diff

    async def write_response(self, message, writer):
        response = self.serializer.serialize(message, 'TicTacToe')
        writer.write(response)
        try:
            await writer.drain()
        except ConnectionResetError:
            self.connected = False
            writer.close()

    async def session_lost(self, board, writer):
        await self.write_response(
            self.game_ctor(
                board=None,
                error=SessionLost(
                    self.dparser,
                    'Session {} is no longer active'.format(
                        board.session,
                    )).proto(),
            ),
            writer,
        )
        del self.sessions[board.sessions]
        self.connected = False

    async def illegal_move(self, writer):
        await self.write_response(
            self.game_ctor(
                board=None,
                error=InvalidMove(
                    self.dparser,
                    'That move is not allowed'
                ).proto(),
            ),
            writer,
        )
        self.connected = False

    async def game_error(self, error, writer):
        await self.write_response(
            self.game_ctor(
                board=None,
                error=eror.proto(),
            ),
            writer,
        )
        self.connected = False

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
                if message.board.session:
                    try:
                        known = self.sessions[message.board.session]
                    except KeyError:
                        await self.session_lost(message.board, writer)
                        return
                    diff = self.diff_boards(known, message.board)
                    if len(diff) != 1:
                        await self.illegal_move(writer)
                        return
            except Exception as e:
                print('what happened: {}'.format(e))
                traceback.print_tb(sys.exc_info()[2])
            else:
                break

        if self.connected:
            try:
                self.game.move(message.board, self.dparser)
                session = message.board.session or self.gensession()
                if is_board_full(message.board):
                    del self.sessions[message.board.session]
                else:
                    self.sessions[session] = message.board
                message = self.game_ctor(
                    board=self.board_ctor(
                        cells=message.board.cells,
                        session=session,
                    ),
                    error=None,
                )
                await self.write_response(message, writer)
            except TicTacToeError as e:
                await self.game_error(e, writer)
                return

    async def handle(self, reader, writer):
        self.connected = True
        while self.connected:
            await self.turn(reader, writer)


server = TicTacToeServer()


async def handler(reader, writer):
    await server.handle(reader, writer)
