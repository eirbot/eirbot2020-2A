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
    arrived = State("arrived")
    blocked = State("blocked")

    # Classic use
    start = ready.to(waiting)
    goto = waiting.to(moving)
    arrive = moving.to(arrived)
    wait = arrived.to(waiting)

    # Errors and block
    block = blocked.from_(waiting, arrived, moving)

    def __init__(self, com, protocol):
        self.x = None
        self.y = None
        self.theta = None
        self.com = com
        self.protocol = protocol
        self.gp2s = None
        self.parent_strategy = None

        # Adding features to the protocol
        features = [
            Feature("get_pos", 0, self.on_get_pos),
            Feature("set_pos", 1, None),
            Feature("goto", 2, None),
            Feature("stop", 3, None),
            Feature("panic", 4, None),
            Feature("get_gp2", 5, self.on_get_gp2),
            Feature("ping", 6, self.on_get_pos),
        ]

        self.protocol.add_features(features)

    def on_goto(self, x, y, theta):
        logging.debug("robot:goto:(x=%s, y=%s, t=%s)", x, y, theta)
        goto = self.protocol.features["goto"]
        self.com.ask(goto, x, y, theta)

    def on_get_pos(self):
        logging.debug("robot:on_get_pos: reading 12 bytes")
        pos = self.com.serial.read(12)
        x, y, theta = struct.unpack("fff", pos)
        logging.debug("robot:on_get_pos:(x=%s, y=%s, t=%s)", x, y, theta)
        self.x = x
        self.y = y
        self.theta = theta

    def on_get_gp2(self):
        logging.debug("robot:on_get_gp2: reading 12 bytes")
        pos = self.com.serial.read(12)
        self.gp2s = struct.unpack("fff", pos)
        logging.debug("robot:on_get_gp2:(%s)", self.gp2)
