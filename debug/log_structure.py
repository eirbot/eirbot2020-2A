#!/usr/bin/env python

class LogStructure:

    def __init__(self):
        self.logs = []
        self.last_log_index = 0
        self.save_filename = ".log"

    def append(self, time, tag, data):
        log = {"time": time, "tag": tag, "data": data}

        with open(self.save_filename, "a") as f:
            f.write(log2String(log))

        self.logs.append(log)

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
        for log in self.logs:
            string += log2String(log)
        return string[:-1]

def log2String(log_dict):
    return "{time} : {tag:8} : {data}\n".format(**log_dict)
