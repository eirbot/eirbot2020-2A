#include "GP2.hpp"

#define GP2_COEF (1.0f / 0.93f)

// 1.0  -> 6 cm
// 0.4  -> 20 cm
// 0.72 -> 10 cm
// O.19 -> 50 cm

GP2::GP2() {}
GP2::GP2(AnalogIn *_input, float _distance_seuil) {
  input = _input;
  distance_seuil = _distance_seuil;
  activated = 1; // Activated
}

int GP2::detected() { return input->read() * GP2_COEF > distance_seuil; }

float GP2::get_Distance() { return (input->read() * GP2_COEF); }

void GP2::debug() { printf("GP2 : %f, %f", input->read(), get_Distance()); }

int GP2::is_active() { return activated; }

void GP2::activate() { activated = 1; }

void GP2::deactivate() { activated = 0; }

/*
 * Return the id of the first GP2 that is triggered
 * or 0 else.
 * */
int detected_all(GP2 gp2_list[], int gp2_nb) {
  for (int i = 0; i < gp2_nb; i++) {
    if (gp2_list[i].is_active() && gp2_list[i].detected())
      return i + 1;
  }
  return 0;
}

// void frontGP2Action(Navigateur nav) {}

// void backGP2Action(Navigateur nav) {}

// void leftGP2Action(Navigateur nav) {}

// void rightGP2Action(Navigateur nav) {}
