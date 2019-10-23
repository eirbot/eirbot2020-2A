#include "base.hpp"
#include "config.hpp"
#include "encoder.hpp"
#include "mbed.h"
#include "position.hpp"
#include <cmath>

// const float D = 13900; // 0.2200684f * 367.0f; // mm.tick^-1 * distance entre
// roues

float sg(float val) { return (val < 0.0f) ? -1.0f : 1.0f; }

Position::Position() {}

Position::Position(Encoder *_encod_l, Encoder *_encod_r) {
  x = 0;
  y = 0;
  theta = 0;
  encod_l = _encod_l;
  encod_r = _encod_r;
}

float Position::get_x() { return x; }

float Position::get_y() { return y; }

float Position::get_theta() { return theta; }

/**
 * Nouveau modèle de déplacement élémentaire centré autour du milieu des deux
 * encodeurs.
 * */
void Position::update(short vitesses[]) {
  short dl = encod_l->diff();
  short dr = encod_r->diff();

  vitesses[0] = dr;
  vitesses[1] = dl;

  float del = dl / ENCODEUR_ECHELLE; // mouvement de l'encodeur gauche en mètres
  float der = dr / ENCODEUR_ECHELLE; // mouvement de l'encodeur droit en mètres

  // On avance de la motié de la longueur
  x += (del + der) / 4 * cos(theta);
  y += (del + der) / 4 * sin(theta);

  // On tourne
  theta = modulo_angle_absolu((der - del) / DIAMETER + theta);

  // On réavance de la motié de la longueur
  x += (del + der) / 4 * cos(theta);
  y += (del + der) / 4 * sin(theta);
}

void Position::init() { reset(); }

void Position::reset() {
  x = 0;
  y = 0;
  theta = 0;
}
