# #!/usr/bin/env python

import threading

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
        """Start a listener in the background"""
        self.running = True
        threading.Thread(target=self.start_listening_thread).start()

    def start_listening_thread(self):
        """Start a listener in the background"""
        while self.running:
            data = self.sock.recv(1024).decode()
            if data:
                self.on_recv(data)

if __name__ == '__main__':
    s = ClientConnection('127.0.0.1', 1234)
    s.start()
