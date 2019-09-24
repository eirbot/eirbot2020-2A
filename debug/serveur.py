#!/usr/bin/env python

import socket
import sys
import time

import connection

class ServerConnection(connection.Connection):

    def __init__(self, port):
        super().__init__()
        try:
            self.sock.bind(('localhost', port))
        except socket.error as msg:
            print('Bind failed. {} : {}'.format(msg.errno, msg.args))
            sys.exit()

    def start(self):
        self.sock.listen(1)
        self.conn, addr = self.sock.accept()
        # print("Inbound connection from {}".format(addr))

        if self.conn:
            self.running = True

            while self.running:
                data = self.conn.recv(1024).decode()
                if data:
                    self.on_recv(data)
        else:
            print("No connection established")

    def send(self, data):
        """Send data to the last conn"""
        self.conn.send(data.encode())

if __name__ == '__main__':
    s = ServerConnection(1234)
    s.start()
    while True:
        s.send("test")
