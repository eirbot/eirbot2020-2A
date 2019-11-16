/*
 * TODO
 * -
 */

#include "testers.hpp"

void length_calibration(Serial *const ser, Qei *const qei_l, Qei *const qei_r) {
  short val_l = qei_l->getQei();
  short val_r = qei_r->getQei();
  float dl = 0.0f;
  float dr = 0.0f;
  float length = 0.0f;
  ser->printf("length measurement parameters:\n\r");
  ser->printf("L: %fm\n\r", LENGTH_TEST);
  ser->printf("pattern:\tdl+dr/2\tlength\n\r");
  ser->printf("starting length measurement...\n\r");
  ser->getc();
  while (!ser->readable()) {
    dl = qei_l->getQei(&val_l);
    dr = qei_r->getQei(&val_r);
    length += (dl + dr) / 2;
    ser->printf("%f\t%f\n\r", dl + dr / 2, length);
  }
}

void angle_calibration(Serial *const ser, Qei *const qei_l, Qei *const qei_r) {
  short val_l = qei_l->getQei();
  short val_r = qei_r->getQei();
  float dl = 0.0f;
  float dr = 0.0f;
  float angle = 0.0f;
  ser->printf("angle measurement parameters:\n\r");
  ser->printf("A: %fPI rad\n\r", ANGLE_TEST);
  ser->printf("pattern:\tdl+dr/2\tangle\n\r");
  ser->printf("starting angle measurement...\n\r");
  ser->getc();
  while (!ser->readable()) {
    dl = qei_l->getQei(&val_l);
    dr = qei_r->getQei(&val_r);
    angle += (dl - dr) / 2;
    ser->printf("%f\t%f\n\r", dl - dr / 2, angle);
  }
}

void transfer(Serial *const ser, LMD18200 *const motor_l,
              LMD18200 *const motor_r, Qei *const qei_l, Qei *const qei_r) {
  float t = 0.0f;
  short val_l = qei_l->getQei();
  short val_r = qei_r->getQei();
  float dl = 0.0f;
  float dr = 0.0f;
  Timer t_timer;
  motor_l->setDirection(DIR_FORWARD);
  motor_r->setDirection(DIR_FORWARD);
  ser->printf("impulse response parameters:\n\r");
  ser->printf("t1: %fs, pwm1: %f, t2:%fs, pwm2: %f\n\r", T1_transfer, PWM1,
              T2_transfer, PWM2);
  ser->printf("pattern:\n\rt\tdl\tdr\n\r");
  ser->printf("starting impulse response...\n\r");
  ser->getc();
  t_timer.start();
  motor_l->put(PWM1);
  motor_r->put(PWM1);
  while (t < T1_transfer && !ser->readable()) {
    t = t_timer.read();
    dl = qei_l->getQei(&val_l);
    dr = qei_r->getQei(&val_r);
    ser->printf("%f\t%f\t%f\n\r", t, dl, dr);
  }
  motor_l->put(PWM2);
  motor_r->put(PWM2);
  while (t < T2_transfer && !ser->readable()) {
    t = t_timer.read();
    dl = qei_l->getQei(&val_l);
    dr = qei_r->getQei(&val_r);
    ser->printf("%f\t%f\t%f\n\r", t, dl, dr);
  }
  motor_l->put(0.0f);
  motor_r->put(0.0f);
}

void transfer2(Serial *const ser, LMD18200 *const motor_l,
               LMD18200 *const motor_r, Qei *const qei_l, Qei *const qei_r) {
  float t = 0.0f;
  short val_l = qei_l->getQei();
  float dl = 0.0f;
  Timer t_timer;
  ser->printf("impulse response parameters:\n\r");
  ser->printf("t1: %fs, pwm1: %f, t2:%fs, pwm2: %f\n\r", T1_transfer, PWM1,
              T2_transfer, PWM2);
  ser->printf("starting left motor impulse response...\n\r");
  ser->getc();
  t_timer.start();
  motor_l->put(PWM1);
  while (t < T1_transfer && !ser->readable()) {
    t = t_timer.read();
    dl = qei_l->getQei(&val_l);
    ser->printf("%f\t%f\n\r", t, dl);
  }
  motor_l->put(PWM2);
  while (t < T2_transfer && !ser->readable()) {
    t = t_timer.read();
    dl = qei_l->getQei(&val_l);
    ser->printf("%f\t%f\n\r", t, dl);
  }
  short val_r = qei_r->getQei();
  float dr = 0.0f;
  motor_l->put(0.0f);
  t_timer.stop();
  t_timer.reset();
  ser->printf("starting right motor impulse response...\n\r");
  ser->getc();
  t = t_timer.read();
  t_timer.start();
  motor_r->put(PWM1);
  while (t < T1_transfer && !ser->readable()) {
    t = t_timer.read();
    dr = qei_r->getQei(&val_r);
    ser->printf("%f\t%f\n\r", t, dr);
  }
  motor_r->put(PWM2);
  while (t < T2_transfer && !ser->readable()) {
    t = t_timer.read();
    dr = qei_r->getQei(&val_r);
    ser->printf("%f\t%f\n\r", t, dr);
  }
  motor_r->put(0.0f);
}

void pid_test(Serial *const ser, SpeedBlock *const speed_block) {
  float t = 0.0f;
  float SPspeed_l = 1.0f;
  float SPspeed_r = 1.0f;
  float PVspeed_l = 0.0f;
  float PVspeed_r = 0.0f;
  float pwm_l = 0.0f;
  float pwm_r = 0.0f;
  ser->printf("pattern:\n\riteration\tSPspeed_l\tPVspeed_l\tpwm_l\t\t"
              "SPspeed_r\tPVspeed_r\tpwm_r\n\r");
  ser->printf("starting speed test...\n\r");
  speed_block->setSpeed(SPspeed_l, SPspeed_r);
  ser->getc();
  while (1) {
    speed_block->getPVspeed(&PVspeed_l, &PVspeed_r);
    speed_block->getPWM(&pwm_l, &pwm_r);
    ser->printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\n\r", t, SPspeed_l, PVspeed_l,
                pwm_l, SPspeed_r, PVspeed_r, pwm_r);
    t += 0.003f;
    speed_block->refresh();
    ser->getc();
  }
}

void speed_test(Serial *const ser, SpeedBlock *const speed_block) {
  float t = 0.0f;
  float SPspeed_l = 50.0f;
  float SPspeed_r = 50.0f;
  float PVspeed_l = 0.0f;
  float PVspeed_r = 0.0f;
  float pwm_l = 0.0f;
  float pwm_r = 0.0f;
  Timer t_timer;
  ser->printf("pattern:\n\rtime\t\tSPspeed_l\tPVspeed_l\tpwm_l\t\t"
              "SPspeed_r\tPVspeed_r\tpwm_r\n\r");
  ser->printf("starting speed test...\n\r");
  ser->printf("starting translation test...\n\r");
  ser->getc();
  t_timer.start();
  speed_block->start();
  while (!ser->readable()) {
    t = t_timer.read();
    speed_block->getPVspeed(&PVspeed_l, &PVspeed_r);
    speed_block->getPWM(&pwm_l, &pwm_r);
    ser->printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\n\r", t, SPspeed_l, PVspeed_l,
                pwm_l, SPspeed_r, PVspeed_r, pwm_r);
    // SPspeed_l = min(SPspeed_l + DELTA_V, MAX_SP);
    // SPspeed_r = min(SPspeed_r + DELTA_V, MAX_SP);
    speed_block->setSpeed(SPspeed_l, SPspeed_r);
    wait(0.01f);
  }
  ser->getc();
  speed_block->reset();
  ser->printf("starting rotation test...\n\r");
  t_timer.stop();
  t_timer.reset();
  SPspeed_l = 0.0f;
  SPspeed_r = 0.0f;
  ser->getc();
  t_timer.start();
  speed_block->start();
  while (!ser->readable()) {
    t = t_timer.read();
    speed_block->getPVspeed(&PVspeed_l, &PVspeed_r);
    speed_block->getPWM(&pwm_l, &pwm_r);
    ser->printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\n\r", t, SPspeed_l, PVspeed_l,
                pwm_l, SPspeed_r, PVspeed_r, pwm_r);
    SPspeed_l = min(SPspeed_l + DELTA_V, MAX_SP);
    SPspeed_r = -min(-SPspeed_r + DELTA_V, MAX_SP);
    speed_block->setSpeed(SPspeed_l, SPspeed_r);
    wait(0.1f);
  }
  speed_block->reset();
}

void square(Serial *const ser, SpeedBlock *const speed_block) {
  float SPspeed;
  Timer t_timer;
  ser->printf("starting square example...\n\r");
  t_timer.start();
  speed_block->start();
  while (!ser->readable()) {
    speed_block->setSpeed(0.0f, 0.0f);
    SPspeed = 0.0f;
    t_timer.reset();
    while (t_timer.read() < 1.0f && !ser->readable())
      ;
    t_timer.reset();
    while (t_timer.read() < 2.5f && !ser->readable()) {
      SPspeed = min(SPspeed + DELTA_V, MAX_SP);
      speed_block->setSpeed(SPspeed, SPspeed);
    }
    speed_block->setSpeed(0.0f, 0.0f);
    SPspeed = 0.0f;
    t_timer.reset();
    while (t_timer.read() < 1.0f && !ser->readable())
      ;
    t_timer.reset();
    while (t_timer.read() < 0.85f && !ser->readable()) {
      SPspeed = min(SPspeed + DELTA_V, MAX_SP);
      speed_block->setSpeed(SPspeed, -SPspeed);
    }
  }
  speed_block->reset();
}

void test_odometry(Serial *const ser, Odometry *const odometry) {
  float x = 0;
  float y = 0;
  float a = 0;
  odometry->reset();
  ser->printf("starting odometry test...\n\r");
  ser->printf("pattern:\n\rx\t\ty\t\ta\t\tx(m)\t\ty(m)\t\ta(pi.rad)\n\r");
  ser->getc();
  odometry->start();
  while (!ser->readable()) {
    odometry->getPos(&x, &y, &a);
    ser->printf("%8f\t%8f\t%8f\t%8f\t%8f\t%f\r", x, y, a, x / TICKS_PM,
                y / TICKS_PM, a / TICKS_PRAD / PI);
  }
  odometry->reset();
}

void test_navigator(Serial *const ser, Navigator *const navigator,
                    Odometry *const odometry) {
  float x, y, a;
  navigator->reset();
  ser->printf("starting odometry test...\n\r");
  ser->printf("pattern:\n\rx\t\ty\t\ta\t\tx(m)\t\ty(m)\t\ta(pi.rad)\n\r");
  navigator->setDst(0.5f, 0.5f, PI / 2);
  ser->getc();
  odometry->start();
  navigator->start();
  while (!ser->readable()) {
    odometry->getPos(&x, &y, &a);
    ser->printf("%8f\t%8f\t%8f\t%8f\t%8f\t%f\r", x, y, a, x / TICKS_PM,
                y / TICKS_PM, a / TICKS_PRAD / PI);
  }
  odometry->reset();
}

void test_strat(Serial *const ser, Navigator *const navigator,
                Odometry *const odometry) {
  float x = 0;
  float y = 0;
  float a = 0;
  navigator->reset();
  odometry->setPos(1.03f * TICKS_PM, 1.31 * TICKS_PM, -PI / 2 * TICKS_PRAD);
  odometry->getPos(&x, &y, &a);
  ser->printf("starting strat test...\n\r");
  ser->printf("pattern:\n\rx(m)\t\ty(m)\t\ta(pi.rad)\n\r");
  ser->getc();
  navigator->start();

  float x_dst[] = {
      1.10f * TICKS_PM, 0.85f * TICKS_PM, 0.58f * TICKS_PM, 0.69f * TICKS_PM,
      0.45f * TICKS_PM, 0.44f * TICKS_PM, 0.79f * TICKS_PM, 1.37f * TICKS_PM,
      0.51f * TICKS_PM, 1.38f * TICKS_PM, 1.03f * TICKS_PM, 0.51f * TICKS_PM,
      0.51f * TICKS_PM, 0.66f * TICKS_PM, 0.28f * TICKS_PM, 0.28f * TICKS_PM,
      0.42f * TICKS_PM, 0.32f * TICKS_PM, 0.42f * TICKS_PM, 1.28f * TICKS_PM,
      1.44f * TICKS_PM, 1.0f * TICKS_PM,
  };
  float y_dst[] = {
      0.85f * TICKS_PM,  0.95f * TICKS_PM,  1.32f * TICKS_PM,
      0.95f * TICKS_PM,  0.95f * TICKS_PM,  1.30f * TICKS_PM,
      0.15f * TICKS_PM,  0.29f * TICKS_PM,  1.17f * TICKS_PM,
      0.24f * TICKS_PM,  0.22f * TICKS_PM,  0.73f * TICKS_PM,
      1.13f * TICKS_PM,  0.75f * TICKS_PM,  -0.13f * TICKS_PM,
      -0.27f * TICKS_PM, -0.77f * TICKS_PM, -0.75f * TICKS_PM,
      -0.75f * TICKS_PM, 0.23f * TICKS_PM,  0.19f * TICKS_PM,
      0.00f * TICKS_PM,
  };
  float a_dst[] = {
      0.86f * PI * TICKS_PRAD,
      NAN, // 0.84f*PI*TICKS_PRAD,
      NAN, // 0.67f*PI*TICKS_PRAD,
      -0.96f * PI * TICKS_PRAD,
      0.54f * PI * TICKS_PRAD,
      NAN, // 0.53f*PI*TICKS_PRAD,
      0.06f * PI * TICKS_PRAD,
      0.77f * PI * TICKS_PRAD,
      NAN, // 0.75f*PI*TICKS_PRAD,
      -0.97f * PI * TICKS_PRAD,
      0.63f * PI * TICKS_PRAD,
      NAN, // 0.52f*PI*TICKS_PRAD,
      NAN, // 0.52f*PI*TICKS_PRAD,
      NAN, // 0.36f*PI*TICKS_PRAD,
      0.50f * PI * TICKS_PRAD,
      NAN, // 0.50f*PI*TICKS_PRAD,
      1.00f * PI * TICKS_PRAD,
      1.00f * PI * TICKS_PRAD,
      0.29f * PI * TICKS_PRAD,
      -0.06f * PI * TICKS_PRAD,
      -0.06f * PI * TICKS_PRAD,
      0.00f * PI * TICKS_PRAD,
  };
  int len = 22;

  for (int i = 0; i < len; i++) {
    navigator->setDst(x_dst[i], y_dst[i], a_dst[i]);
    while (!navigator->ready() && !ser->readable()) {
      odometry->getPos(&x, &y, &a);
      ser->printf("%8f\t%8f\t%f\r", x / TICKS_PM, y / TICKS_PM,
                  a / TICKS_PRAD / PI);
    }
  }
  ser->getc();
  navigator->reset();
  while (!ser->readable()) {
    odometry->getPos(&x, &y, &a);
    ser->printf("%8f\t%8f\t%f\r", x / TICKS_PM, y / TICKS_PM,
                a / TICKS_PRAD / PI);
  }
}

void test_real(Serial *const ser, Strat *const strat, Odometry *const odometry,
               DigitalIn *const side, DigitalIn *const key) {
  float x, y, a;
  strat->reset();
  ser->printf("starting real test...\n\r");
  ser->printf("init...\n\r");
  strat->init(side, key);
  ser->printf("run: side=%d\n\r", side->read());
  while (strat->run() && !ser->readable()) {
    odometry->getPos(&x, &y, &a);
    ser->printf("%8f\t%8f\t%f\t", x / TICKS_PM, y / TICKS_PM,
                a / TICKS_PRAD / PI);
    ser->printf("%8.8x\r", gp2Obstacle());
  }
  ser->printf("\n\rend...\n\r");
}
