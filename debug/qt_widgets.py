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

    def __init__(self, logs):
        super().__init__()

        self.logs = logs

        layout = QtWidgets.QHBoxLayout()

        self.x_label = QtWidgets.QLabel()
        self.y_label = QtWidgets.QLabel()
        self.t_label = QtWidgets.QLabel()

        layout.addWidget(self.x_label)
        layout.addWidget(self.y_label)
        layout.addWidget(self.t_label)

        self.setLayout(layout)

    def update_control_values(self):
        """The position is intended to be send as (x,y,theta)"""
        position = self.logs.get_last_log_by_tag("POS")
        position_data = position["data"]
        if position_data:
            x, y, t = position[1:-1].split(",")
        else:
            x = y = t = "None"

        self.x_label.setText(x)
        self.y_label.setText(y)
        self.t_label.setText(t)
