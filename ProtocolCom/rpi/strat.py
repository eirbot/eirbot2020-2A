#!/usr/bin/env python3

from statemachine import StateMachine, State


class Strat(StateMachine):

    # STATES
    init = State("init", initial=True)
    p1 = State("pos1", value=(0, 0, 0))
    p2 = State("pos2", value=(0, 0, 0))
    p3 = State("pos3", value=(0, 0, 0))
    end = State("end")

    # TRANSITIONS
    start = init.to(p1)
    next = p1.to(p2) | p2.to(p3) | p3.to(end)
    end = end.from_(p1, p2, p3)

    def __init__(self, robot):
        super().__init__()
        self.robot = robot

    def meta_goto(self):
        self.robot.goto(*self.current_state_value)

    on_enter_pos1 = meta_goto
    on_enter_pos2 = meta_goto
    on_enter_pos3 = meta_goto
