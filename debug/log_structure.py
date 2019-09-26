#!/usr/bin/env python

class LogStructure:

    def __init__(self):
        self.logs = []
        self.last_log_index = 0

    def append(self, time, tag, data):
        self.logs.append({
            "time": time,
            "tag": tag,
            "data": data
        })

    def get_tag(self, tag):
        # TODO : implement filter by tag
        return self.logs

    def get_new(self):
        logs = self.logs[self.last_log_index - 1:]
        self.last_log_index = len(self.logs)
        return logs

    def __str__(self):
        string = ""
        for i, log in enumerate(self.logs, 1):
            string += " {}. {}\n".format(i, log)
        return string[:-1]
