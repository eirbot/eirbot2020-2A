#!/usr/bin/env python3

from com import Feature
import logging
import struct
from statemachine import StateMachine, State

logging.basicConfig(level=logging.DEBUG)


class Robot(StateMachine):
    """
    The class that represent the robot.
    The robot has the following status:
    MOVIN, ARRIVED, BLOCKED
    """

    ready = State("ready", initial=True)
    waiting = State("waiting")
    moving = State("move")
    arrival = State("arrived")
    blocked = State("blocked")

    # Classic use
    start = ready.to(waiting)
    goto = waiting.to(moving)
    arrive = moving.to(arrival)
    wait = arrival.to(waiting)

    # Errors and block
    block = blocked.from_(waiting, arrival, moving)

    def __init__(self, protocol=None):
        self.x = None
        self.y = None
        self.theta = None
        self.gp2s = None
        self.protocol = None

    def on_goto(self, x, y, theta):
        logging.debug("robot:goto:(x=%s, y=%s, t=%s)", x, y, theta)
        goto = self.protocol.features["goto"]
        self.protocol.com.ask(goto, x, y, theta)
