
#ifndef NAVIGATOR_HPP
#define NAVIGATOR_HPP

#include <mbed.h>
#include <math.h>
#include <common.hpp>
#include <speed_block.hpp>
#include <odometry.hpp>

#define PERIOD_POS 0.02f

#define A_DIST 0.003f
#define A_ANGLE 0.01f

#define THRESH_DIST (0.01f*TICKS_PM)
#define THRESH_ANGLE (PI/64*TICKS_PRAD)

#define CEIL_DIST 70
#define CEIL_ANGLE 70

class Navigator
{
public:
	Navigator(Odometry* odometry, SpeedBlock* _speed_block);
	~Navigator();
	void reset();
	void start();
	void setDst(float const _x, float const _y, float const _a);
	bool ready();
	bool obstacle;
private:
	void refresh();
	float x;
	float y;
	float a;
	bool ready_val;
	Odometry* const odometry;
	SpeedBlock* const speed_block;
	Ticker ticker;
};

#endif
