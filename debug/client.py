#!/usr/bin/env python

import cmd
import asyncio

class ClientProtocol(asyncio.Protocol):

    def connection_made(self, transport):
        peername = transport.get_extra_info('peername')
        print('Connection from {}'.format(peername))
        self.transport = transport

    def data_received(self, data):
        message = data.decode()
        print('Data received: {!r}'.format(message))

        print('Send: {!r}'.format(message))
        self.transport.write(data)

class DebugShell(cmd.Cmd):
    """Shell used to interact with the RPI"""

    intro = 'Welcome to the debug shell.   Type help or ? to list commands.\n'
    prompt = '> '
    file = None

    def __init__(self, transport, protocol):
        super().__init__()
        self.protocol = protocol
        self.transport = transport

    def do_start(self, arg):
        'Start a process'
        self.send("start {}".format(arg))

    def do_set(self, arg):
        'Set the var to a value'
        args = arg.split(' ')

        if not len(args) == 2:
            print("Not the right number of arguments")
        else:
            var_name = args[0]
            var_value = args[1]

            # TODO: Send set command to RPI

    def do_exit(self, arg):
        'Exit the DebugShell'
        return True

    def send(self, message):
        self.transport.write(message.encode())

async def main():
    loop = asyncio.get_event_loop()
    transport, protocol = await loop.create_connection(lambda: ClientProtocol(), '127.0.0.1', 8888)
    DebugShell(transport, protocol).cmdloop()

if __name__ == '__main__':
    asyncio.run(main())
