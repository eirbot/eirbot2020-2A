#include "position.hpp"
#include <queue>

#ifndef __PID_H_
#define __PID_H_

float calculerAngle(float x1, float y1, float x2, float y2);

class PID { // PID asservissement en position
public:
  PID();
  PID(float p, float i, float d, float erreurSeuil, float accumulateurSeuil,
      float erreur_limite_acc, float deltaRampe);
  void AccumulerErreur(float erreur);
  float calculerConsigne();
  float getConsigne();
  virtual float calculerErreur() = 0; // A override dans les classes filles
  void stockerErreur();
  float getDerivee();
  float deltaRampe;

  void reset();

  float erreur;
  float erreurSeuil;
  float erreur_limite_acc;

  float accumulateur;
  std::queue<float> fifo;

  float derivee_data[3];

  float last_time;

  float p;
  float i;
  float d;

  float consigne;

  int compteur_acc;

  short actionFinished;

  Timer time;

  float accumulateurSeuil;
};

class PIDDistance : public PID {
public:
  PIDDistance();
  PIDDistance(float p, float i, float d, float erreurSeuil,
              float accumulateurSeuil, Position *position,
              float erreur_limite_acc, float deltaRampe);

  float calculerErreur();
  void setCommande(float x, float y);

  float commande_x;
  float commande_y;
  Position *pos;
};

class PIDAngle : public PID {
public:
  PIDAngle();
  PIDAngle(float p, float i, float d, float erreurSeuil,
           float accumulateurSeuil, Position *position, float erreur_limite_acc,
           float deltaRampe);

  float calculerErreur();
  void setCommande(float theta);

  float commande_theta;
  Position *pos;
};

class PIDVitesse : public PID {
public:
  PIDVitesse();
  PIDVitesse(float p, float i, float d, float erreurSeuil,
             float accumulateurSeuil, Encoder *encoder, float coef,
             float erreur_limite_acc, float deltaRampe);

  float calculerErreur();
  void updateVitesse(short v);
  void setCommande(float theta);

  float commande_vitesse = 0;
  float vitesse;
  float coef; // Coef to pass from command to vitesse
  Encoder *encod;
};
#endif // __PID_H_
