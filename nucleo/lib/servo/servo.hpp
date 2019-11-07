
#ifndef SERVO_H
#define SERVO_H

#define SERVO_PWM_PRESCAL 59 // -1 pour avoir pile 50Hz sur l'oscillo, no lo sé why
#define SERVO_PWM_PERIOD 60000 // 60 000 = 100% de pwm
#define SERVO_INIT 4250 // Anciennement 0.0645f avec PwmOUT
#define SERVO_LEFT 1900 // Anciennement 0.025f avec PwmOUT
#define SERVO_RIGHT 6550 // Anciennement 0.0645f avec PwmOUT

#include <mbed.h>

void servoTimerInit();
void servoSetPwmDuty(int dutyCycle);

#endif
