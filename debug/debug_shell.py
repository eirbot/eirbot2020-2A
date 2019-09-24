#!/usr/bin/env python

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

    def do_send(self, arg):
        if arg:
            self.connection.send(arg)

    def do_logs(self, arg):
        'Show logs received'
        if not arg:
            print(self.connection.logs)

    def do_exit(self, arg):
        'Exit the DebugShell'
        return True

    def send(self, message):
        self.connection.send(message)
