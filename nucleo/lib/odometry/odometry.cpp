/*
 * TODO
 * Documentation
 * Add all constants in config.hpp
 */

#include "odometry.hpp"


Odometry::Odometry(Qei* _qei_l, Qei* _qei_r):
	qei_l(_qei_l),
	qei_r(_qei_r)
{
	reset();
}

Odometry::~Odometry()
{
	
}

void Odometry::reset()
{
	x = 0;
	y = 0;
	a = 0;
	ticker.detach();
}

void Odometry::start()
{
	qei_l_val = qei_l->getQei();
	qei_r_val = qei_r->getQei();
	ticker.attach(callback(this, &Odometry::refresh), PERIOD_ODOMETRY);
}

void Odometry::getPos(float* const _x, float* _y, float* _a)
{
	*_x = x;
	*_y = y;
	*_a = a;
}

void Odometry::setPos(float _x, float _y, float _a)
{
	x = _x;
	y = _y;
	a = _a;
}

void Odometry::refresh()
{
	float dleft = (float)qei_l->getQei(&qei_l_val);
	float dright = (float)qei_r->getQei(&qei_r_val);
	float dr = (dleft+dright)/2.0f;
	float da = (dright-dleft)/2.0f;
	a += da/2;
	x += cos(a/TICKS_PRAD)*dr;
	y += sin(a/TICKS_PRAD)*dr;
	a += da/2;
	if (abs(a) > PI*TICKS_PRAD) a -= sg(a)*TWOPI*TICKS_PRAD;
}
