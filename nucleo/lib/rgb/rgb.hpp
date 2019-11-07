#ifndef RGB_H
#define RGB_H

#include <mbed.h>

class RGB{
public:
	RGB(PinName r_pin, PinName g_pin, PinName b_pin);
	~RGB();
	void off();
	void setColor(bool r, bool g, bool b);
private:
	DigitalOut r;
	DigitalOut g;
	DigitalOut b;
};

#endif
