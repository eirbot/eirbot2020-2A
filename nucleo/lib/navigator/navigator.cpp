/*
 * TODO
 * Documentation
 * Add all constants in config.hpp
 */

#include "navigator.hpp"


Navigator::Navigator(Odometry* _odometry, SpeedBlock* _speed_block):
	odometry(_odometry),
	speed_block(_speed_block)
{
	reset();
}

Navigator::~Navigator()
{
}

void Navigator::reset()
{
	ticker.detach();
	odometry->reset();
	speed_block->reset();
	obstacle = 0;
}

void Navigator::start()
{
	odometry->start();
	speed_block->start();
	ticker.attach(callback(this, &Navigator::refresh), PERIOD_POS);
	obstacle = 0;
}

void Navigator::setDst(float const _x, float const _y, float const _a)
{
	x = _x;
	y = _y;
	a = (abs(_a) > PI*TICKS_PRAD) ? _a - sg(_a)*TWOPI*TICKS_PRAD : _a;
	ready_val = false;
}

bool Navigator::ready()
{
	return ready_val;
}

void Navigator::refresh()
{
	float x_pos;
	float y_pos;
	float a_pos;
	odometry->getPos(&x_pos, &y_pos, &a_pos);
	float dx = x - x_pos;
	float dy = y - y_pos;
	float r = isNan(dx) || isNan(dy) ? 0.0f : sqrtf(pow(dx, 2) + pow(dy, 2));
	ready_val = false;
	float t = isNan(a) ? 0.0f : a - a_pos;
	if (abs(r) > THRESH_DIST) {
		t = TICKS_PRAD * atan2(dy, dx) - a_pos;
		t = (abs(t) > PI*TICKS_PRAD) ? t - sg(t)*TWOPI*TICKS_PRAD : t;
		if (abs(t) > PI/2*TICKS_PRAD) {
			t = t - sg(t)*PI*TICKS_PRAD;
			r = -r;
		}
	} else {
		t = (abs(t) > PI*TICKS_PRAD) ? t - sg(t)*TWOPI*TICKS_PRAD : t;
		if (abs(t) < THRESH_ANGLE) {
			t = 0.0f;
			ready_val = true;
		}
		r = 0.0f;
	}
	r = sg(r)*min(A_DIST*abs(r), CEIL_DIST);
	r = obstacle ? 0.0f : r;
	t = sg(t)*min(A_ANGLE*abs(t), CEIL_ANGLE);
	speed_block->setSpeed(r-t, r+t);
}
