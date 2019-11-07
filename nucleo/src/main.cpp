#include "main.hpp"
#include <mbed.h>
#include <rgb.hpp>
#include <gp2.hpp>
#include <servo.hpp>
#include <pump.hpp>
#include <qei.hpp>
#include <pid.hpp>
#include <lmd18200.hpp>
#include <speed_block.hpp>
#include <odometry.hpp>
#include <navigator.hpp>
#include <strat.hpp>
#include <bluetooth_controller.hpp>

#ifdef DEBUG
#include <debug.hpp>
#include <testers.hpp>
#endif


#ifdef DEBUG
int err = NO_ERROR;
Serial ser(USBTX, USBRX);
#else
int err = 0;
#endif

float coef_err_l[] = {0.01* 0.8659f, 0.01* -0.7513f, 0.01* -0.8629, 0.01* 0.7543};
float coef_co_l[] = {1.0f, -0.9107f, -0.1491f, 0.05982f};
float coef_err_r[] = {0.01* 0.8659f, 0.01* -0.7513f, 0.01* -0.8629, 0.01* 0.7543};
float coef_co_r[] = {1.0f, -0.9107f, -0.1491f, 0.05982f};

DigitalOut led(LED2);
DigitalIn side(SIDE_PIN);
DigitalOut ev(PB_9);
DigitalOut po(PC_9);
DigitalOut poki(PC_3, PullUp);
DigitalIn waiting_key(KEY_PIN, PullUp);
RGB rgb(LED_R_PIN, LED_G_PIN, LED_B_PIN);
Serial seg(SEG_TX, SEG_RX);
Qei qei_l(ENCODER_TIM_LEFT, &err);
Qei qei_r(ENCODER_TIM_RIGHT, &err);
Pid pid_l(coef_err_l, NB_COEF_ERR, coef_co_l, NB_COEF_CO);
Pid pid_r(coef_err_r, NB_COEF_ERR, coef_co_r, NB_COEF_CO);
LMD18200 motor_l(PWM_L, DIR_L, BREAK_L, DIR_FWD_L, PERIOD_PWM);
LMD18200 motor_r(PWM_R, DIR_R, BREAK_R, DIR_FWD_R, PERIOD_PWM);
SpeedBlock speed_block(&qei_l, &pid_l, &motor_l, &qei_r, &pid_r, &motor_r);
Odometry odometry(&qei_l, &qei_r);
Navigator navigator(&odometry, &speed_block);
Strat strat(&navigator, &odometry, &seg, &rgb);


int main()
{
	strat.reset();
	rgb.setColor(1, 1, 1);
	led = 1;
	seg.baud(9600);
	servoTimerInit();
	initPump(PUMP_PIN, EVALVE_PIN);
	initGp2(GP2_FL, GP2_FC, GP2_FR, GP2_RL, GP2_RC, GP2_RR);
	wait(3.0f);
#ifdef DEBUG
	ser.baud(115200);
	ser.printf("\r\nstart\r\n");
	ser.printf("error code: %d\r\n", err);
#ifdef LENGTH_CALIB
	length_calibration(&ser, &qei_l, &qei_r);
#endif
#ifdef ANGLE_CALIB
	angle_calibration(&ser, &qei_l, &qei_r);
#endif
#ifdef TRANSFER
	transfer(&ser, &motor_l, &motor_r, &qei_l, &qei_r);
#endif
#ifdef PID
	pid_test(&ser, &speed_block);
#endif
#ifdef SPEED
	speed_test(&ser, &speed_block);
#endif
#ifdef BTPS3
	bluetooth_controller_PS3(&rgb, &speed_block, &ev, &po, &poki);
#endif
#ifdef SQUARE
	square(&ser, &speed_block);
#endif
#ifdef ODOMETRY
	test_odometry(&ser, &odometry);
#endif
#ifdef NAVIGATOR
	test_navigator(&ser, &navigator, &odometry);
#endif
#ifdef STRAT
	test_strat(&ser, &navigator, &odometry);
#endif
#ifdef REAL
	test_real(&ser, &strat, &odometry, &side, &waiting_key);
#endif
#endif
	strat.reset();
	while (1);
}
