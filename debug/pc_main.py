#!/usr/bin/env python
""" Main script to be run by the computer. """

import client
import log_structure
import debug_shell
from PySide2 import QtCore, QtWidgets, QtGui
from qt_widgets import Console, Table, ControlPanel


logs = log_structure.LogStructure()
c = client.ClientConnection('127.0.0.1', 1222, logs)
c.start()

shell = debug_shell.DebugShell(c)

#
# QT APP
#

app = QtWidgets.QApplication([])

mainLayout = QtWidgets.QGridLayout()

control_pannel_widget = ControlPanel(logs)

mainLayout.addWidget(Table(), 0, 0)
mainLayout.addWidget(control_pannel_widget, 0, 1)

console_widget = Console(shell, logs)
mainLayout.addWidget(console_widget, 1, 0, 1, 2)

timer = QtCore.QTimer()
timer.timeout.connect(console_widget.update_logs)
timer.timeout.connect(control_pannel_widget.update_control_values)
timer.start(200) # unit : ms

window = QtWidgets.QWidget()
window.setLayout(mainLayout)
window.show()

app.exec_()
