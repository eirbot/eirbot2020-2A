#include "../base.hpp"
#include "../config.hpp"
#include "navigateur.hpp"
#include <math.h>
#include <stdio.h>

extern int debug_monitor;
extern int move;
extern int running;

Navigateur::Navigateur(Position *_position, PwmOut *_m_l, PwmOut *_m_r,
                       DigitalOut *_d_l, DigitalOut *_d_r, Encoder *encod_l,
                       Encoder *encod_r) {
  position = _position;

  float p_vitesse = 5;
  float k = 0.023;
  // FIXME : Trouver bonnes valeurs de pid.
  PIDDistance _pid_d(0.40, 0.001, 0.0, 0.04, 2, position, 0.4f,
                     0.01); // 0.5cm de précision
  PIDAngle _pid_a(0.48, 0.001, 0.0, 0.02, 1, position, 0.1f, 0.01f); // 1 degré
  // PIDAngle _pid_a(0.03, 0.001, 0.001, 0.02, 0, position);
  PIDVitesse _pid_v_l(p_vitesse * (1 - k), 0.004, 0, 0.001, 3, encod_l, 0.004,
                      1.0f, 1);
  PIDVitesse _pid_v_r(p_vitesse * (1 + k), 0.004, 0, 0.001, 3.
  , encod_r, 0.004,
                      1.0f, 1);

  time.start();

  pid_d = _pid_d;
  pid_a = _pid_a;
  pid_v_l = _pid_v_l;
  pid_v_r = _pid_v_r;
  cible_x = 0;
  cible_y = 0;
  actionFinished = 0;
  compteur = 0;

  consigne_motor_r = 0;
  consigne_motor_l = 0;

  m_l = _m_l;
  m_r = _m_r;
  m_l->period(PWM_PERIOD);
  m_r->period(PWM_PERIOD);
  d_l = _d_l;
  d_r = _d_r;

}

void Navigateur::set_destination(float x, float y) {
  cible_x = x;
  cible_y = y;
  // Cible sert de stokage de la consigne
  pid_d.setCommande(cible_x, cible_y);
}

void Navigateur::reset() {
  position->reset();
  set_destination(0, 0);
  reset_pids();
}

void Navigateur::reset_pids() {
  pid_a.reset();
  pid_d.reset();
  pid_v_l.reset();
  pid_v_r.reset();
}

void Navigateur::update() {

  if (time.read_ms() > 100000 + start_time) {
    m_r->write(0.0f);
    m_l->write(0.0f);
  }else{

  float x = position->get_x();
  float y = position->get_y();
  float theta = position->get_theta();

  dir_r = 1;
  dir_l = 0;

  // Initialisation des consignes

  float distance_cible = sqrt(carre(x - cible_x) + carre(y - cible_y));

  if (updateAngleCons) {
    if (pid_d.actionFinished) {
      compteur++;
      if (compteur > 50) {
        actionFinished = 1;
      }
    } else {
      actionFinished = 0;
      compteur = 0;
    }
  } else {
    if (pid_a.actionFinished) {
      compteur++;
      if (compteur > 10) {
        actionFinished = 1;
      }
    } else {
      actionFinished = 0;
      compteur = 0;
    }
  }

  angle_absolu_destination =
      modulo_angle_absolu(calculerAngle(x, y, cible_x, cible_y));

  angle_relatif = modulo_angle_relatif(angle_absolu_destination - theta);

  if ((abs(angle_relatif) < 0.3) || (abs(abs(angle_relatif) - PI) < 0.3)) {
    triggered = 1;
    dist_cons = updateAngleCons ? pid_d.getConsigne() : 0;
  } else {

    pid_d.reset();
  }

  if ((angle_relatif > PI - 0.7) || (angle_relatif < -PI + 0.7)) {
    sens = ARRIERE;
  } else {
    sens = AVANT;
  }
  //  Calcul Consigne pour angle avec angle cible rafraichie
  if (updateAngleCons) {
    if (sens == AVANT) {
      pid_a.setCommande(angle_absolu_destination);
    } else {
      pid_a.setCommande(modulo_angle_absolu(angle_absolu_destination + PI));
    }
  }

  angle_cons = pid_a.getConsigne();

  dist_cons = min(dist_cons, 0.2);
  angle_cons = min(angle_cons, 0.2f);

  dist_cons = max(dist_cons, -0.2f);
  angle_cons = max(angle_cons, -0.2f);

  /*
    Initialisation des directions pour que les moteurs tournent dans
    le même sens. On inverse ensuite selon la consigne lors de l'appel
    à `limiter_consigne`.
    */
  if (sens == AVANT) {
    cmr = dist_cons + angle_cons; // Consigne moteur droit
    cml = dist_cons - angle_cons; // Consigne moteur gauche
  } else {
    cmr = -dist_cons + angle_cons; // Consigne moteur droit
    cml = -dist_cons - angle_cons; // Consigne moteur gauche
  }

  pid_v_r.setCommande(cmr);
  pid_v_l.setCommande(cml);

  consigne_motor_r = pid_v_r.getConsigne();
  consigne_motor_l = pid_v_l.getConsigne();

  cmr = consigne_motor_r;
  cml = consigne_motor_l;

  limiter_consigne(&cmr, &dir_r);
  limiter_consigne(&cml, &dir_l);

  if (move) {
    m_r->write(cmr);
    m_l->write(cml);
  }

  *d_r = dir_r;
  *d_l = dir_l;
  }
}

/*
 * Tourne le robot de `angle`. Angle représente l'angle relatif,
 * il peut être positif ou négatif.
 */
void Navigateur::rotate_by(float angle) {
  float theta = position->get_theta();
  float angle_dest = modulo_angle_absolu(theta + angle);

  pid_a.setCommande(angle_dest);
  updateAngleCons = 0;

  while (!actionFinished) {
    if (debug_monitor) {
      debug();
    }
  }

  actionFinished = 0;
  compteur = 0;
}

/*
 * Tourne le robot jusqu'à `angle`. Angle représente l'angle relatif,
 * il peut être positif ou négatif.
 */
void Navigateur::rotate_to(float angle_dest) {
  pid_a.setCommande(angle_dest);
  updateAngleCons = 0;

  while (!actionFinished) {
    if (debug_monitor) {
      debug();
    }
  }

  actionFinished = 0;
  compteur = 0;
}

/*
 * Avance de `distance` mètres.
 * TODO : Tester cette fonction
 */
void Navigateur::avancer(float distance) {
  float x = position->get_x();
  float y = position->get_y();
  float theta = position->get_theta();

  float new_x = x + distance * cos(theta);
  float new_y = y + distance * sin(theta);

  set_destination(new_x, new_y);
  updateAngleCons = 1;

  while (!actionFinished) {
    if (debug_monitor) {
      debug();
    }
  }

  actionFinished = 0;
  compteur = 0;
}

/*
 * Va à la position `x`, `y` mètres.
 */
void Navigateur::go_to(float cx, float cy) {
  set_destination(cx, cy);
  updateAngleCons = 1;

  while (!actionFinished) {
    if (debug_monitor) {
      debug();
    }
  }

  actionFinished = 0;
  compteur = 0;
}

void Navigateur::print_pos() {
  printf("\n\rx:%.2f y:%.2f t:%.2f ", position->get_x(), position->get_y(),
         convert_degree(position->get_theta()));
}

void Navigateur::updatePos() {
  short vitesses[2] = {0, 0};
  position->update(vitesses);
  pid_v_r.updateVitesse(vitesses[0]);
  pid_v_l.updateVitesse(vitesses[1]);
}

void Navigateur::debug() {
  printf("(%.2f,%.2f,%.2f)(%.1f,%.2f,%.2f) %s [%.2f|%.2f] R:%.2f "
         "[A:%.2f,D:%.2f] %f, %f\n",
         position->get_x(), position->get_y(),
         convert_degree(position->get_theta()), cible_x, cible_y,
         angle_absolu_destination, sens == AVANT ? "Av" : "Ar",
         consigne_motor_l, consigne_motor_r,
         convert_degree(modulo_angle_relatif(angle_relatif)), angle_cons,
         dist_cons, pid_v_l.accumulateur, pid_v_r.accumulateur);
}
