/*
 * TODO
 * Documentation
 */

#include "lmd18200.hpp"


LMD18200::LMD18200(PinName const _pwm, PinName const _dir, PinName const _br,
		bool const _dir_fwd, float const _period):
	dir_fwd(_dir_fwd),
	pwm(_pwm),
	dir(_dir),
	br(_br)
{
	pwm.period(_period);
	setPwm(0.0f);
	setDirection(DIR_FORWARD);
	setBreak(BREAK_OFF);
}

LMD18200::~LMD18200()
{
	
}

float LMD18200::getPwm()
{
	return pwm.read();
}

bool LMD18200::getDir()
{
	return dir;
}

bool LMD18200::getBreak()
{
	return br;
}

void LMD18200::setPwm(float const _duty)
{
	float duty = min(_duty, 1.0f);
	pwm.write(duty);
}

void LMD18200::setDirection(bool const _dir)
{
	dir = (_dir && !dir_fwd) || (!_dir && dir_fwd);
}

void LMD18200::setBreak(bool const _break)
{
	br = _break;
}

