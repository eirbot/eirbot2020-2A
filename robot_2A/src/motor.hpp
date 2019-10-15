#ifndef MOTOR_H
#define MOTOR_H

#include <cstdint>
/* Class to work with a motor alone. */
class Motor {
   
public:
  Motor(void);

  /* Set the Ration of the PWM. The value can go from 0
   * to 2^16 */ 
  void setRatio(int16_t ratio);

private:
  
};

#endif MOTOR_H
