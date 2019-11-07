#ifndef GP2_HPP
#define GP2_HPP

#include "mbed.h"

void initGp2(PinName _fl, PinName _fc, PinName _fr, PinName _rl, PinName _rc,
		PinName _rr);
bool gp2Obstacle(int mask);
int gp2Obstacle();

#endif
