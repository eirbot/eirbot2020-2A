"""
Ce fichier contient une implémentation manuelle d'un protocole de communication
sériale. Il est pensé pour permettre l'échange d'informations sous un certain
format et repose essentiellement sur l'utilisation du module `struct`.
"""

import serial
import struct
import logging

SERIAL_PORT = "/dev/ttyACM0"

logging.basicConfig(level=logging.DEBUG)




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


def ask(feature, *args):
    payload = feature.identifier.to_bytes(1, "little")
    for arg in args:
        if isinstance(arg, int):
            payload += struct.pack("i", arg)
        elif isinstance(arg, float):
            payload += struct.pack("f", arg)
    logging.debug("%s: Sending %s", feature.name, payload)
    return payload


def parse(payload):
    feature_id = payload[0]
    return FEATURES[feature_id].handle(payload[1:])


def simple_handle_decode(decode_str):
    """
    Return a simple handler that return a tuple depending on `decode_str`
    """
    return lambda x: struct.unpack(decode_str, x)


get_pos = Feature("get_pos", 0, simple_handle_decode("fff"))
set_pos = Feature("set_pos", 1, simple_handle_decode("c"))
goto = Feature("goto", 2, simple_handle_decode("c"))
stop = Feature("stop", 3, simple_handle_decode("c"))
panic = Feature("panic", 4, lambda: print("Panic received"))

FEATURES = [get_pos, set_pos, goto, stop, panic]


def main():
    s = serial.Serial(SERIAL_PORT,baudrate=115200)
    sending = 0
    if sending:
        s.write(ask(get_pos))
        res = s.read(13)
        print(parse(res))
    else:
        s.write(ask(set_pos, 0.1, 0.3, 0.4))

main()
