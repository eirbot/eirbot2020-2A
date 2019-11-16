#ifndef LMD18200_HPP
#define LMD18200_HPP

#include "mbed.h"
#include <common.hpp>
#include <output.hpp>

#define DIR_FORWARD 0
#define DIR_BACKWARD 1
#define BREAK_OFF 0
#define BREAK_ON 1
#define MAX_DUTY 0.80f

class LMD18200 : public Output<float> {
public:
  LMD18200(PinName const _pwm, PinName const _dir, PinName const _br,
           bool const _dir_fwd, float const _period);
  ~LMD18200();
  float getPwm();
  bool getDir();
  bool getBreak();
  void put(float const _pwm);
  void setDirection(bool const _dir);
  void setBreak(bool const _br);

private:
  bool const dir_fwd;
  PwmOut pwm;
  DigitalOut dir;
  DigitalOut br;
};

#endif
