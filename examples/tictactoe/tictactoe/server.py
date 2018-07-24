# -*- coding: utf-8 -*-

import pkg_resources

from os import path

from protopy.parser import BinParser
from protopy.serializer import Serializer


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

    async def turn(self, reader, writer):
        data, message = b'', None
        while True:
            try:
                try:
                    chunk = await reader.read(self.buffsize)
                    print('received {} bytes'.format(len(chunk)))
                    if not chunk:
                        self.connected = False
                        break
                    data += chunk
                except ConnectionResetError:
                    writer.close()
                    return
                message = self.parser.parse(self.proto_def, 'TicTacToe', data)
            except Exception as e:
                print('what happened: {}'.format(e))
            else:
                break

        if self.connected:
            response = self.serializer.serialize(message, 'TicTacToe')
            print("send: {} = {}".format(message, response))
            writer.write(data)
            try:
                await writer.drain()
            except ConnectionResetError:
                writer.close()

    async def handle(self, reader, writer):
        self.connected = True
        while self.connected:
            await self.turn(reader, writer)

        # writer.close()


server = TicTacToeServer()


async def handler(reader, writer):
    await server.handle(reader, writer)
