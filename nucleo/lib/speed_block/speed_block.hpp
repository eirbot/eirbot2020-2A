
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "mbed.h"
#include "pid.hpp"
#include "qei.hpp"
#include "lmd18200.hpp"


#define PERIOD_SPEED_PID 0.003f

class SpeedBlock
{
public:
	SpeedBlock(Qei* const _qei_l, Pid* const _pid_l, LMD18200* const _motor_l,
			Qei* const _qei_r, Pid* const _pid_r, LMD18200* const _motor_r);
	~SpeedBlock();
	void reset();
	void pause();
	void start();
	void getSPspeed(float* _SPspeed_l, float* _SPspeed_r);
	void getPVspeed(float* _PVspeed_l, float* _PVspeed_r);
	void getPWM(float* _duty_l, float* _duty_r);
	void setSpeed(float const _SPspeed_l, float const _SPspeed_r);
	void refresh();
private:
	float SPspeed_l;
	float SPspeed_r;
	float PVspeed_l;
	float PVspeed_r;
	float duty_l;
	float duty_r;
	short qei_l_val;
	short qei_r_val;
	Ticker ticker;
	Qei* const qei_l;
	Pid* const pid_l;
	LMD18200* const motor_l;
	Qei* const qei_r;
	Pid* const pid_r;
	LMD18200* const motor_r;
};


#endif
