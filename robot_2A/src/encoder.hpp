#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "mbed.h"
#include <cstdint>

class Encoder
{
public:
  Encoder(TIM_TypeDef* timer);

  /* Return the absolute distance recorded by the encoder 
   * since the beginning of the program.
   * unit: encoder tick */
  int32_t getDistance(void);

public:
  void reset();
  short diff();

private:
  TIM_TypeDef* TIM;
  uint16_t lastValue;
};

extern Encoder enc_l;
extern Encoder enc_r;

#endif // __ENCODER_H_
