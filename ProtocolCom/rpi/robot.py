#!/usr/bin/env python3

from com import Feature
import logging
import struct

logging.basicConfig(level=logging.DEBUG)


class Robot:
    """
    The class that represent the robot.
    The robot has the following status:
    MOVING, ARRIVED, BLOCKED
    """

    def __init__(self, com, protocol):
        self.x = None
        self.y = None
        self.theta = None
        self.com = com
        self.protocol = protocol
        self.gp2s = None

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

    def goto(self, x, y, theta):
        logging.debug("robot:goto:(x=%s, y=%s, t=%s)", x, y, theta)
        goto = self.protocol.features["goto"]
        self.com.ask(goto, x, y, theta)
        self.status = "MOVING"

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
