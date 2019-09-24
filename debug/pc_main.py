#!/usr/bin/env python

import client
import log_structure
import debug_shell

logs = log_structure.LogStructure()
c = client.ClientConnection('127.0.0.1', 1234, logs)
c.start()

shell = debug_shell.DebugShell(c)
shell.cmdloop()

# Start QT
# TODO : implement QT

