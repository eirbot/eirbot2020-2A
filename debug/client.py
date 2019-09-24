# #!/usr/bin/env python

import connection

class ClientConnection(connection.Connection):

    def __init__(self, host, port=1234):
       super().__init__()
       self.sock.connect((host, port))
       self.logs = []

    def on_recv(self, data):
        """Add data received in a log structure"""
        self.logs.append(data)
        print(self.logs)

    def start(self):
        self.running = True

        while self.running:
            data = self.sock.recv(1024).decode()
            if data:
                self.on_recv(data)

if __name__ == '__main__':
    s = ClientConnection('127.0.0.1', 1234)
    s.start()
