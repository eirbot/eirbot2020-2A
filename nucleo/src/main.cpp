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

#include <shell.hpp>

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

struct ShellSerialIn {
	Serial& ser;
	ShellSerialIn(Serial& ser) : ser(ser) {}
	unsigned int readable(void) {
		return ser.readable() ? 1 : 0;
	}

	char get(void) {
		return ser.getc();
	}
};

struct ShellSerialOut {
	Serial& ser;
	ShellSerialOut(Serial& ser) : ser(ser) {}
	void put(char c) {
		ser.putc(c);
	}
};

struct ShellCmd {
  using Func = int (*)(void* obj, int argc, char* argv[]);

  bool run_shell = true;

  struct Main {
    void* obj;
    Func func;

    Main(void* obj, Func func) : obj(obj), func(func) {}

    int operator()(int argc, char* argv[]) {
      return func(obj, argc, argv);
    }

    operator bool(void) {
      return func;
    }
  };

  int echo(int argc, char* argv[]) {

    for (int i = 1 ; i < argc ; i++) {
      ser.printf("%d ", (int)argv[i]);
    }
    ser.printf("\n");

    return 0;
  }

  void exit(void) {
    run_shell = false;
  }

  Main operator[] (char* cmdname) {
    if (std::strcmp(cmdname, "echo") == 0) {
      return Main(this, [](void*obj, int argc, char* argv[]) -> int { return static_cast<ShellCmd*>(obj)->echo(argc, argv); });
    }
    else if (std::strcmp(cmdname, "exit") == 0) {
      return Main(this, [](void* obj, int , char* []) -> int { static_cast<ShellCmd*>(obj)->exit(); return 0; });
    }
    else {
      ser.printf("unknown\n");
      return Main(nullptr, nullptr);
    }
  }
};

ShellSerialIn ssi(ser);
ShellSerialOut sso(ser);
ShellCmd scmd;

Shell<ShellSerialIn, ShellSerialOut, ShellCmd> shell(ssi, sso, scmd);

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

	ser.baud(115200);
	ser.printf("\r\nstart\r\n");
	ser.printf("error code: %d\r\n", err);

	while(scmd.run_shell) {
		shell.update();
	}

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
	strat.reset();
	while (1);
}
