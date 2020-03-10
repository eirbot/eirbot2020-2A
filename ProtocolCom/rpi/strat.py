#!/usr/bin/env python3

from statemachine import StateMachine, State


class Strat(StateMachine):
    initialisation = State("Initialisation", initial=True)
    pos1 = State("pos1", value=(0, 0, 0))
    pos2 = State("pos2", value=(1, 1, 1))
    pos3 = State("pos3", value=(2, 2, 2))
    end = State("end")

    start = initialisation.to(pos1)
    next = pos1.to(pos2) | pos2.to(pos3) | pos3.to(end) | end.to.itself()

    def meta_goto(self):
        print(f"I'm now in state {self.current_state.name}")

    on_enter_pos1 = meta_goto
    on_enter_pos2 = meta_goto
    on_enter_pos3 = meta_goto
