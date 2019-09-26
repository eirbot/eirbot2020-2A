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

HLayout = QtWidgets.QHBoxLayout()
VLayout = QtWidgets.QHBoxLayout()

HLayout.addWidget(Table())
HLayout.addWidget(ControlPanel())

console_widget = Console(shell, logs)

VLayout.addLayout(HLayout)
VLayout.addWidget(console_widget)

timer = QtCore.QTimer()
timer.timeout.connect(console_widget.update_logs)
timer.start(100)

window = QtWidgets.QWidget()
window.setLayout(VLayout)
window.show()

app.exec_()
