/*
 * TODO
 * Documentation
 */

#include "speed_block.hpp"

SpeedBlock::SpeedBlock(Qei* _qei_l, Pid* _pid_l, LMD18200* _motor_l,
		Qei* _qei_r, Pid* _pid_r, LMD18200* _motor_r):
	qei_l(_qei_l),
	pid_l(_pid_l),
	motor_l(_motor_l),
	qei_r(_qei_r),
	pid_r(_pid_r),
	motor_r(_motor_r)
{
	SPspeed_l = 0.0f;
	SPspeed_r = 0.0f;
	PVspeed_l = 0.0f;
	PVspeed_r = 0.0f;
	duty_l = 0.0f;
	duty_r = 0.0f;
}

SpeedBlock::~SpeedBlock()
{

}

void SpeedBlock::reset()
{
	motor_l->setPwm(0.0f);
	motor_r->setPwm(0.0f);
	qei_l->reset();
	qei_r->reset();
	pid_l->reset();
	pid_r->reset();
	ticker.detach();
	SPspeed_l = 0.0f;
	SPspeed_r = 0.0f;
	PVspeed_l = 0.0f;
	PVspeed_r = 0.0f;
	duty_l = 0.0f;
	duty_r = 0.0f;
}

void SpeedBlock::pause()
{
	motor_l->setPwm(0.0f);
	motor_r->setPwm(0.0f);
	ticker.detach();
}

void SpeedBlock::start()
{
	qei_l_val = qei_l->getQei();
	qei_r_val = qei_r->getQei();
	ticker.attach(callback(this, &SpeedBlock::refresh), PERIOD_SPEED_PID);
}

void SpeedBlock::getSPspeed(float* _SPspeed_l, float* _SPspeed_r)
{
	*_SPspeed_l = SPspeed_l;
	*_SPspeed_r = SPspeed_r;
}

void SpeedBlock::getPVspeed(float* _PVspeed_l, float* _PVspeed_r)
{
	*_PVspeed_l = PVspeed_l;
	*_PVspeed_r = PVspeed_r;
}

void SpeedBlock::getPWM(float* _duty_l, float* _duty_r)
{
	*_duty_l = duty_l;
	*_duty_r = duty_r;
}

void SpeedBlock::setSpeed(float const _SPspeed_l, float const _SPspeed_r)
{
	SPspeed_l = _SPspeed_l;
	SPspeed_r = _SPspeed_r;
}

void SpeedBlock::refresh()
{
	PVspeed_l = (float)qei_l->getQei(&qei_l_val);
	PVspeed_r = (float)qei_r->getQei(&qei_r_val);
	float err_l = SPspeed_l - PVspeed_l;
	float err_r = SPspeed_r - PVspeed_r;
	duty_l = limit(pid_l->getPid(err_l, duty_l), -MAX_DUTY, MAX_DUTY);
	duty_r = limit(pid_r->getPid(err_r, duty_r), -MAX_DUTY, MAX_DUTY);
	if (duty_l >= 0.0f) {
		motor_l->setDirection(DIR_FORWARD);
		motor_l->setPwm(duty_l);
	} else {
		motor_l->setDirection(DIR_BACKWARD);
		motor_l->setPwm(-duty_l);
	}
	if (duty_r >= 0.0f) {
		motor_r->setDirection(DIR_FORWARD);
		motor_r->setPwm(duty_r);
	} else {
		motor_r->setDirection(DIR_BACKWARD);
		motor_r->setPwm(-duty_r);
	}
}
