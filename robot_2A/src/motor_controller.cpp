#include "motor_controller.hpp"

MotorController::MotorController(Motor& mot, Encoder& enc)
  : motor(mot), encoder(enc) {
}

void MotorController::setSpeed(float _speed) {
  speed = _speed;
}
