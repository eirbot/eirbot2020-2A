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

        layout = QtWidgets.QGridLayout()

        self.variables_robot = ["x", "y", "theta", "speed"]
        self.set_vars = ["P", "I", "D"] # vars that can be set

        self.labels = [QtWidgets.QLabel() for _ in self.variables_robot + self.set_vars]

        for i in range(len(self.variables_robot)):
            layout.addWidget(self.labels[i], i // 3, i % 3)

        current_nb_row = len(self.variables_robot) // 3 + 1

        for i in range(len(self.set_vars)):
            layout.addWidget(self.labels[i + len(self.variables_robot)],
                             i // 3 + current_nb_row, i % 3)

        self.setLayout(layout)

    def update_control_values(self):
        """The position is intended to be send as (x,y,theta,...)"""
        position = self.logs.get_last_log_by_tag("INFO_ROBOT")["data"]
        if position:
            data_vars = position[1:-1].split(",")
        else:
            data_vars = ["None" for _ in self.variables_robot]

        for value, name in zip(data_vars, self.variables_robot):
            self.labels[self.variables_robot.index(name)].setText("{}: {}".format(name, value))

        for i in range(len(self.set_vars)):
            self.labels[i + len(self.variables_robot)].setText(self.set_vars[i])
