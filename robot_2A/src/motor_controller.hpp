#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <cstdint>
#include "encoder.hpp"
#include "motor.hpp"

/* Class to work with a association of a motor and an encoder.
 * This class does an asserv in speed.
 */
class MotorController {
   
public:
  MotorController(void);

  /* Set the speed of the encoder.
   * unit: radian/s */ 
  void setSpeed(float speed);

private:
  Encoder encoder;
  Motor motor;
  float speed;
 
  
};

#endif MOTOR_CONTROLLER_H
