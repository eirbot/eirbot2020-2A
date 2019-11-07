#include <rgb.hpp>

RGB::RGB(PinName r_pin, PinName g_pin, PinName b_pin):
	r(r_pin),
	g(g_pin),
	b(b_pin)
{
	off();
}

RGB::~RGB()
{
}

void RGB::off()
{
	r = 0;
	g = 0;
	b = 0;
}

void RGB::setColor(bool _r, bool _g, bool _b)
{
	r = _r;
	g = _g;
	b = _b;
}
