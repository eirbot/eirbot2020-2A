/*
 * TODO
 * Documentation
 */

#include <waypoint.hpp>

Waypoint::Waypoint():
	x(0.0f),
	y(0.0f),
	a(0.0f),
	next(NULL),
	action(NULL)
{
}

Waypoint::Waypoint(float const _x, float const _y, float const _a):
	x(_x*TICKS_PM),
	y(_y*TICKS_PM),
	a(abs(_a) > PI ? (_a - sg(a)*2*PI)*TICKS_PRAD : _a * TICKS_PRAD),
	next(NULL),
	action(NULL)
{
}

Waypoint::Waypoint(float const _x, float const _y, float const _a,
		Waypoint* const _next, int (*_action)(Waypoint**, Navigator*,
			Timer*, float*)):
	x(_x*TICKS_PM),
	y(_y*TICKS_PM),
	a(abs(_a) > PI ? (_a - sg(a)*2*PI)*TICKS_PRAD : _a * TICKS_PRAD),
	next(_next),
	action(_action)
{
}

Waypoint::~Waypoint()
{
}
