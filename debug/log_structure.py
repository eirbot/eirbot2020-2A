#!/usr/bin/env python

class LogStructure:

    def __init__(self):
        self.logs = []

    def append(self, log_element):
        # TODO : implement log dict structure
        self.logs.append(log_element)

    def get_tag(self, tag):
        # TODO : implement filter by tag
        return self.logs

    def __str__(self):
        string = ""
        for i, log in enumerate(self.logs, 1):
            string += " {}. {}\n".format(i, log)
        return string[:-1]
