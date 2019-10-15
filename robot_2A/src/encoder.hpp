#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "mbed.h"

class Encoder
{
    public:
        Encoder(TIM_TypeDef* TIM);
        Encoder();
        void reset();
        short get();
        short diff();
        short lastValue;
    private:
        TIM_TypeDef* TIM;
};


void TIM3_EncoderInit();

void TIM4_EncoderInit();

#endif // __ENCODER_H_
