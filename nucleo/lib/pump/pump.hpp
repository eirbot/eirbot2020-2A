#ifndef PUMP_HPP
#define PUMP_HPP

#include "mbed.h"

void initPump(PinName _pump, PinName _evalve);
void activatePump();
void releasePump();

#endif
