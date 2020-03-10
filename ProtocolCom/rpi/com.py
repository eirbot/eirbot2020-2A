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
            handler = FEATURES[feature_id].handle
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
    def __init__(self):
        self.features = {}

    def add_feature(self, feature):
        self.features[feature.name] = feature

    def add_features(self, features):
        for feature in features:
            self.add_feature(feature)


def main():
    import threading

    c = Com(SERIAL_PORT)
    com_thread = threading.Thread(target=c.loop)
    com_thread.start()
    ping = Feature("ping", 0, None)
    c.ask(ping)


if __name__ == "__main__":
    main()
