#include "motor_controller.hpp"

MotorController::MotorController(Output motor_pwm, Input encoder) {
  motor = Motor(motor_pwm);
  encoder = Encoder(encoder);
}

void MotorController::setSpeed(float _speed) {
  speed = _speed;
}
