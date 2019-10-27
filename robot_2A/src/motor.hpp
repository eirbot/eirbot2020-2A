#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"

/* Class to work with a motor alone. */
class Motor {
   
public:
  Motor(PinName pwm_pin, PinName dir_pin, PinName break_pin);

  /* Set the duty cycle of the PWM in float 
     1.0 is 100% in positive direction
     -1.0 is 100% in negative direction
   */
  void setRatio(float ratio);

private:

  PwmOut _pwm;
  DigitalOut _dir;
  DigitalOut _break;

};

extern Motor mot_l;
extern Motor mot_r;

#endif // MOTOR_H
