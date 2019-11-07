#ifndef COMMON_HPP
#define COMMON_HPP

#include "mbed.h"

#define CONST_NAN 54353.423647f

float sg(float const val);
float max(float const val_1, float const val_2);
float min(float const val_1, float const val_2);
float min(float const val_1, float const val_2, float const val_3);
float limit(float const val, float const min, float const max);
bool isNan(float const val);
void blink(DigitalOut* const led);


#endif

