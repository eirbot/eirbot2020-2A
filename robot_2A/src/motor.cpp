#include "motor.hpp"


Motor::Motor(Output _motor_pwm) {
  pwm = _motor_pwm
}

void Motor::setRatio(int16_t _ratio) {
  ratio = _ratio;
}
