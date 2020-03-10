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
    def __init__(self, serial_port):
        self.serial = serial.Serial(serial_port, baudrate=115200)

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
        if feature_id in range(len(FEATURES)):
            FEATURES[feature_id].handle(self.serial)
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


def handle_get_pos(serial):
    logging.debug("handle_get_pos: reading 12 bytes")
    pos = serial.read(12)
    x, y, t = struct.unpack("fff", pos)
    print(f"x: {x}, y: {y}, t: {t}")


def handle_void(serial):
    pass


def simple_handle_decode(decode_str):
    """
    Return a simple handler that return a tuple depending on `decode_str`
    """
    return lambda x: print(struct.unpack(decode_str, x))


get_pos = Feature("get_pos", 0, handle_get_pos)
set_pos = Feature("set_pos", 1, handle_get_pos)
goto = Feature("goto", 2, handle_get_pos)
stop = Feature("stop", 3, handle_get_pos)
panic = Feature("panic", 4, handle_get_pos)

FEATURES = [get_pos, set_pos, goto, stop, panic]


import threading

c = Com(SERIAL_PORT)
com_thread = threading.Thread(target=c.loop)
com_thread.start()
c.ask(get_pos)
