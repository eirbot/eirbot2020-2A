#!/usr/bin/env python

from PySide2 import QtCore, QtWidgets, QtGui

class Console(QtWidgets.QWidget):

    def __init__(self, shell, logs):
        super().__init__()

        self.shell = shell
        self.logs = logs

        self.text_area = QtWidgets.QPlainTextEdit()
        self.shell_input = QtWidgets.QLineEdit()

        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.text_area)
        layout.addWidget(self.shell_input)

        self.setLayout(layout)

        self.shell_input.returnPressed.connect(self.send_command)

    def send_command(self):
        self.shell.onecmd(self.shell_input.text())
        self.shell_input.clear()
        self.update_logs()

    def update_logs(self):
        new_logs = self.logs.get_new()
        if new_logs:
            text = ""
            for log in new_logs:
                text += "{tag:10}: {data}".format(**log)
            self.text_area.appendPlainText(text)


class Table(QtWidgets.QWidget):

    def __init__(self):
        super().__init__()
        label = QtWidgets.QLabel(self)
        label.setText("Table")


class ControlPanel(QtWidgets.QWidget):

    def __init__(self):
        super().__init__()
        label = QtWidgets.QLabel(self)
        label.setText("ControlPanel")
