#ifndef TESTERS_HPP
#define TESTERS_HPP


#include <mbed.h>
#include <qei.hpp>
#include <lmd18200.hpp>
#include <speed_block.hpp>
#include <odometry.hpp>
#include <navigator.hpp>
#include <waypoint.hpp>
#include <strat.hpp>

#define LENGTH_TEST 5.0f
void length_calibration(Serial* const ser, Qei* const qei_l, Qei* const qei_r);

#define ANGLE_TEST 10.0f
void angle_calibration(Serial* const ser, Qei* const qei_l, Qei* const qei_r);

#define T1_transfer 3.0f
#define T2_transfer 7.0f
#define PWM1 0.2f
#define PWM2 0.5f
void transfer(Serial* const ser, LMD18200* const motor_l,
		LMD18200* const motor_r, Qei* const qei_l, Qei* const qei_r);
void transfer2(Serial* const ser, LMD18200* const motor_l,
		LMD18200* const motor_r, Qei* const qei_l, Qei* const qei_r);

#define DELTA_V 1.0f
#define MAX_SP 30.0f
void pid_test(Serial* const ser, SpeedBlock* const speed_block);
void speed_test(Serial* const ser, SpeedBlock* const speed_block);
void square(Serial* const ser, SpeedBlock* const speed_block);

void test_odometry(Serial* const ser, Odometry* const odometry);

void test_navigator(Serial* const ser, Navigator* const navigator,
		Odometry* const odometry);

void test_strat(Serial* const ser, Navigator* const navigator,
		Odometry* const odometry);

void test_real(Serial* const ser, Strat* const strat, Odometry* const odometry,
		DigitalIn* const side, DigitalIn* const key);

#endif
