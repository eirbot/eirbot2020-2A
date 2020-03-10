#!/usr/bin/env python3

import threading as th
from com import Com, Protocol
from robot import Robot


def init_com():
    SERIAL_PORT = "/dev/ttyACM1"
    com = Com(SERIAL_PORT)
    com_thread = th.Thread(target=com.loop)
    com_thread.start()
    return com


def main():
    # Initialise communitation with the nucleo
    com = init_com()
    # Initialise communitation with the nucleo
    protocol = Protocol()
    # Initialise robot object
    robot = Robot(com, protocol)


if __name__ == "__main__":
    main()
