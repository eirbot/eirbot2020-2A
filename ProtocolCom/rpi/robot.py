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
    block = blocked.from_(waiting, moving)

    def __init__(self, strat=None, protocol=None):
        super().__init__()
        self.x = None
        self.y = None
        self.theta = None
        self.gp2s = None
        self.protocol = protocol
        self.strat = strat

    def on_goto(self, x, y, theta):
        logging.debug("robot:goto:(x=%s, y=%s, t=%s)", x, y, theta)
        goto = self.protocol.features["goto"]
        self.protocol.com.ask(goto, x, y, theta)

    def on_enter_arrived(self):
        self.wait()
        self.strat.next()
