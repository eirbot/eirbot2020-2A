#!/usr/bin/env python
""" Main script to be run by the computer. """

import client
import log_structure
import debug_shell
from PySide2 import QtCore, QtWidgets, QtGui
from qt_widgets import Console, Table, ControlPanel


logs = log_structure.LogStructure()
c = client.ClientConnection('127.0.0.1', 1234, logs)
c.start()

shell = debug_shell.DebugShell(c)

#
# QT APP
#

app = QtWidgets.QApplication([])

mainLayout = QtWidgets.QGridLayout()

mainLayout.addWidget(Table(), 0, 0)
mainLayout.addWidget(ControlPanel(), 0, 1)

console_widget = Console(shell, logs)
mainLayout.addWidget(console_widget, 1, 0, 1, 2)

timer = QtCore.QTimer()
timer.timeout.connect(console_widget.update_logs)
timer.start(200) # unit : ms

window = QtWidgets.QWidget()
window.setLayout(mainLayout)
window.show()

app.exec_()
