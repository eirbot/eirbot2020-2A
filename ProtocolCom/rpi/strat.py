#!/usr/bin/env python3

from statemachine import StateMachine, State
import logging

logging.basicConfig(level=logging.DEBUG)


class Strat(StateMachine):

    # STATES
    init = State("init", initial=True)
    p1 = State("pos1", value=(1, 1, 1))
    p2 = State("pos2", value=(2, 2, 2))
    p3 = State("pos3", value=(3, 3, 3))
    end = State("end")

    # TRANSITIONS
    start = init.to(p1)
    next = p1.to(p2) | p2.to(p3) | p3.to(end)

    def __init__(self, robot):
        super().__init__()
        self.robot = robot

    def meta_goto(self):
        logging.debug(
            f"strat: Next position is {self.current_state.name} at {self.current_state.value}"
        )
        self.robot.goto(*self.current_state.value)

    on_enter_p1 = meta_goto
    on_enter_p2 = meta_goto
    on_enter_p3 = meta_goto

    def on_start(self):
        logging.debug("strat: START")
        self.robot.start()

    def on_enter_end(self):
        logging.debug("strat: FINISHED")
