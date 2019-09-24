#!/usr/bin/env python

import socket

class Connection:
    """Interface for connection, meant to be use as a client or a server, and to
    be specified in child class"""

    def __init__(self, running=False):
        self.running = running
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.conn = None

    def start(self):
        pass

    def send(self, data):
        """Send data to the unique other if exist"""
        self.sock.send(data.encode())

    def close(self):
        """Method to call to stop the server"""
        self.running = False

    def on_recv(self, data):
        """Method called when data is received by the connection. Data is
        already decoded"""
        print("Data received: \"{}\"".format(data))

