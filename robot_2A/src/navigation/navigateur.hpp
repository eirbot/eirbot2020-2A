#include "../pid.hpp"
#include "../position.hpp"
#include "mbed.h"
#include "vecteur2D.hpp"

#define NAV_PRECISION 0.01f // Précision de la position en mètres
#define PWM_PERIOD 0.000033F

enum Sens { AVANT, ARRIERE };

/**
 * Classe pour le navigateur.
 * Celui-ci est capable de piloter le robot par mouvement élémentaire pour le
 * conduire jusqu'au point donné.
 */
class Navigateur {
public:
  Navigateur();
  Navigateur(Position *position, PwmOut *m_l, PwmOut *m_r, DigitalOut *_d_l,
             DigitalOut *_d_r, Encoder *encod_l, Encoder *encod_r);
  /**
   * Définit "cible" comme nouveau point à atteindre par le navigateur.
   */
  void set_destination(float x, float y);
  void update();
  void print_pos();
  void debug();
  Position *position;
  void rotate_by(float angle);
  void avancer(float distance);
  void reset();
  void reset_pids();
  void updatePos();
  void go_to(float cx, float cy);
  void rotate_to(float angle_dest);
  PIDVitesse pid_v_l;
  PIDVitesse pid_v_r;
  PIDDistance pid_d;
  PIDAngle pid_a;
  enum Sens sens;
  int actionFinished;
  Timer time;
  float start_time;

private:
  float cible_x;
  float cible_y;
  float angle_reel;
  PwmOut *m_l;
  PwmOut *m_r;
  DigitalOut *d_l;
  DigitalOut *d_r;
  int dir_l;
  int dir_r;
  int updateAngleCons;
  float angle_cons;
  float dist_cons;
  float angle_absolu_destination;
  float angle_relatif;
  int triggered;
  float cmr;
  float cml;
  int compteur;
  float consigne_motor_l;
  float consigne_motor_r;
};
