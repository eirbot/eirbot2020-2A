#include "motor.hpp"

#include <math.h>

Motor::Motor(PinName pwm_pin, PinName dir_pin, PinName break_pin)
  : _pwm(pwm_pin), _dir(dir_pin), _break(break_pin) {
}

void Motor::setRatio(float ratio) {
  _pwm.write(abs(ratio));
  _dir = (ratio <= 0);
}
