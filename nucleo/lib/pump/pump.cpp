#include <pump.hpp>

DigitalOut* pump;
DigitalOut* evalve;

void initPump(PinName _pump, PinName _evalve)
{
	pump = new DigitalOut(_pump);
	evalve = new DigitalOut(_evalve);
	*pump = 1;
	*evalve = 1;
}

void activatePump()
{
	*evalve = 1;
	*pump = 0;
}

void releasePump()
{
	*pump = 1;
	*evalve = 0;
	wait(0.1f);
	*evalve = 1;
}
