#!/usr/bin/env python3

import threading as th
from com import Com, Protocol
from robot import Robot
from strat import Strat


def init_com():
    SERIAL_PORT = "/dev/ttyACM1"
    com = Com(SERIAL_PORT)
    com_thread = th.Thread(target=com.loop)
    com_thread.start()
    return com


def main():
    # Initialise les éléments
    com = init_com()
    robot = Robot()
    strat = Strat(robot)
    protocol = Protocol(com, robot, strat)
    robot.protocol = protocol

    strat.start()


if __name__ == "__main__":
    main()
