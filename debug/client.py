#!/usr/bin/env python

import cmd

class DebugShell(cmd.Cmd):
    """Shell used to interact with the RPI"""

    intro = 'Welcome to the debug shell.   Type help or ? to list commands.\n'
    prompt = '> '
    file = None

    def do_start(self, arg):
        'Start a process'
        send("start {}".format(arg))

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
        return

def send(args):
    pass

if __name__ == '__main__':
    DebugShell().cmdloop()
