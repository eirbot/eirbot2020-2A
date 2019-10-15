#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "mbed.h"
#include <cstdint>

class Encoder
{
public:
  Encoder(TIM_TypeDef* TIM);

  /* Return the absolute distance recorded by the encoder 
   * since the beginning of the program.
   * unit: encoder tick */
  int32_t getDistance(void);
private:
  void reset();
  short lastValue;
  short diff();
  TIM_TypeDef* TIM;
};


void TIM3_EncoderInit();

void TIM4_EncoderInit();

#endif // __ENCODER_H_
