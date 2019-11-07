#include <pump.hpp>

AnalogIn* fl;
AnalogIn* fc;
AnalogIn* fr;
AnalogIn* rl;
AnalogIn* rc;
AnalogIn* rr;

void initGp2(PinName _fl, PinName _fc, PinName _fr, PinName _rl, PinName _rc,
		PinName _rr)
{
	fl = new AnalogIn(_fl);
	fc = new AnalogIn(_fc);
	fr = new AnalogIn(_fr);
	rl = new AnalogIn(_rl);
	rc = new AnalogIn(_rc);
	rr = new AnalogIn(_rr);
}

bool gp2Obstacle(int mask)
{
	return ((fl->read() > (float)((mask & 0x00f00000) >> 20)/16.0f)
			|| (fc->read() > (float)((mask & 0x000f0000) >> 16)/16.0f)
			|| (fr->read() > (float)((mask & 0x0000f000) >> 12)/16.0f)
			|| (rl->read() > (float)((mask & 0x00000f00) >> 8)/16.0f)
			|| (rc->read() > (float)((mask & 0x000000f0) >> 4)/16.0f)
			|| (rr->read() > (float)((mask & 0x0000000f) >> 0)/16.0f));
}

int gp2Obstacle()
{
	return ((((int)(fl->read() * 16.0f) & 0x0000000f) << 20)
			| (((int)(fc->read() * 16.0f) & 0x0000000f) << 16)
			| (((int)(fr->read() * 16.0f) & 0x0000000f) << 12)
			| (((int)(rl->read() * 16.0f) & 0x0000000f) << 8)
			| (((int)(rc->read() * 16.0f) & 0x0000000f) << 4)
			| (((int)(rr->read() * 16.0f) & 0x0000000f) << 0));
}
