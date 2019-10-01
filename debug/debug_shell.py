#!/usr/bin/env python
"""
DebugShell, a shell used to interact with a server hosted on a robot.
"""

import cmd

class DebugShell(cmd.Cmd):
    """Shell used to interact with the RPI"""

    intro = 'Welcome to the debug shell.   Type help or ? to list commands.\n'
    prompt = '> '
    file = None

    def __init__(self, connection):
        super().__init__()
        self.connection = connection

    def do_start(self, arg):
        'Start a process'
        self.send("COMMAND", "start")

    def do_set(self, arg):
        """
        Set the var to a value.
        Syntax : set <var> <value>
        """
        args = arg.split(' ')

        if not len(args) == 2:
            print("Not the right number of arguments")
        else:
            var_name = args[0]
            var_value = args[1]

            # TODO: Send set command to RPI

    def do_send(self, arg):
        """
        Send a message to the server
        Syntax : send <message>
        """
        if arg:
            self.send("MSG_FROM_PC", arg)

    def do_logs(self, arg):
        """
        Show logs received.
        Syntax : logs
        """
        if not arg:
            print(self.connection.logs)

    def do_exit(self, arg):
        """
        Exit the DebugShell
        Syntax : exit
        """
        return True

    def send(self, tag, message):
        """
        Send data to the server with a specific tag.
        """
        self.connection.send("{} {}".format(tag, message))

    def do_simulate(self, arg):
        """
        Ask the server to reply a specific message and a specific tag.
        """
        self.send("SIMULATE", arg)
