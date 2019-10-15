#ifndef __GP2_H_
#define __GP2_H_

#include "mbed.h"

class GP2 {
public:
  GP2(AnalogIn *input, float distance);
  GP2();

  int detected();
  float get_Distance();
  void debug();
  float real_distance;
  int is_active();
  void activate();
  void deactivate();

private:
  AnalogIn *input;
  int activated;
  float distance_seuil;
};

int detected_all(GP2 *gp2_list, int gp2_nb);

#endif // __GP2_H_
