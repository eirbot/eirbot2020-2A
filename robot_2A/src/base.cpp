#include "base.hpp"
#include "config.hpp"
#include <math.h>

float carre(float a) { return a * a; }

float modulo_angle_absolu(float angle) {

  while (angle > 2 * PI) {
    angle -= 2 * PI;
  }

  while (angle < 0) {
    angle += 2 * PI;
  }

  return angle;
}

float modulo_angle_relatif(float angle) {
  while (angle > PI) {
    angle -= 2 * PI;
  }

  while (angle < -PI) {
    angle += 2 * PI;
  }

  return angle;
}

float convert_degree(float angle) { return angle / PI * 180; }

void limiter_consigne(float *consigne, int *direction) {
  if (*consigne < 0) {
    *direction == 1 ? *direction = 0 : *direction = 1;
    *consigne = abs(*consigne);
  }
  // Commenté car géré directement dans la fonction update
  // consigne = min(*consigne, CONSIGNE_MAX);
  *consigne = 0.05f + min(*consigne, 0.3);
}

float min(float a, float b) { return (a > b) ? b : a; }

float max(float a, float b) { return (a < b) ? b : a; }
