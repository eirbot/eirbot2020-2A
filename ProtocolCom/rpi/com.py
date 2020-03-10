"""
Ce fichier contient une implémentation manuelle d'un protocole de communication
sériale. Il est pensé pour permettre l'échange d'informations sous un certain
format et repose essentiellement sur l'utilisation du module `struct`.
"""

import serial
import struct
import logging

SERIAL_PORT = "/dev/ttyACM1"

logging.basicConfig(level=logging.DEBUG)


class Com:
    def __init__(self, serial_port, protocol=None):
        self.serial = serial.Serial(serial_port, baudrate=115200)
        self.protocol = protocol

    def ask(self, feature, *args):
        payload = feature.identifier.to_bytes(1, "little")
        for arg in args:
            if isinstance(arg, int):
                payload += struct.pack("i", arg)
            elif isinstance(arg, float):
                payload += struct.pack("f", arg)
        logging.debug("%s: Sending %s", feature.name, payload)
        self.serial.write(payload)

    def loop(self):
        logging.debug("Waiting data from serial")
        while True:
            request = self.serial.read(1)
            logging.debug("RECEIVED: %s", request)
            self.parse(request.decode())
            ack = self.serial.read(1)
            logging.debug("RECEIVED ack")

    def parse(self, payload):
        feature_id = ord(payload[0])
        if feature_id in range(len(self.protocol.features)):
            handler = self.protocol.features[feature_id].handle
            if handler:
                handler(self.serial)
        else:
            logging.error("parse: id %d not recognized", feature_id)


class Feature:
    """
    Une action à demander sur le protocole sérial.
    name: le nom de l'action, utile uniquement lors du débug
    identifier: l'entier sur un octet identifiant de façon unique l'action
    """

    def __init__(self, name, identifier, handler):
        self.name = name
        self.identifier = identifier
        self.handle = handler


class Protocol:
    def __init__(self, com, robot):
        self.robot = robot
        self.com = com
        self.features = {
            "get_pos": Feature("get_pos", 0, self.on_get_pos),
            "set_pos": Feature("set_pos", 1, None),
            "goto": Feature("goto", 2, None),
            "stop": Feature("stop", 3, None),
            "panic": Feature("panic", 4, None),
            "get_gp2": Feature("get_gp2", 5, self.on_get_gp2),
            "ping": Feature("ping", 6, self.on_get_pos),
            "arrival": Feature("arrival", 7, self.on_arrival),
        }

    def on_get_pos(self):
        logging.debug("robot:on_get_pos: reading 12 bytes")
        pos = self.com.serial.read(12)
        x, y, theta = struct.unpack("fff", pos)
        logging.debug("robot:on_get_pos:(x=%s, y=%s, t=%s)", x, y, theta)
        self.robot.x = x
        self.robot.y = y
        self.robot.theta = theta

    def on_get_gp2(self):
        logging.debug("protocol:on_get_gp2: reading 12 bytes")
        pos = self.com.serial.read(12)
        self.robot.gp2s = struct.unpack("fff", pos)
        logging.debug("protocol:on_get_gp2:(%s)", self.gp2)

    def on_arrival(self):
        self.robot.arrive()


def main():
    import threading

    c = Com(SERIAL_PORT)
    com_thread = threading.Thread(target=c.loop)
    com_thread.start()
    ping = Feature("ping", 0, None)
    c.ask(ping)


if __name__ == "__main__":
    main()
