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
        logs = self.logs[self.last_log_index:]
        self.last_log_index = len(self.logs)
        return logs

    def get_last_log_by_tag(self, tag):
        if self.logs:
            for i in range(len(self.logs), 0, -1):
                if self.logs[i-1]["tag"] == tag:
                    return self.logs[i-1]
        return {
            "time" :"",
            "tag"  :"",
            "data" :"",
        }

    def __str__(self):
        string = ""
        for i, log in enumerate(self.logs, 1):
            string += " {}. {}\n".format(i, log)
        return string[:-1]
