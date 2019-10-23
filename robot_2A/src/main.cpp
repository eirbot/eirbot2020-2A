#include "GP2.hpp"
#include "base.hpp"
#include "config.hpp"
#include "encoder.hpp"
#include "mbed.h"
#include "navigation/navigateur.hpp"
#include "navigation/vecteur2D.hpp"
#include "pid.hpp"
#include "position.hpp"
#include <math.h>

/* Class objects */

DigitalOut led(LED3);
Serial usb(USBTX, USBRX);

DigitalOut sideLed1(PD_0);
DigitalOut sideLed2(PD_1);

Encoder enc_l(TIM3); // PC6  - PC7 ou PA6 - PA7 ou PB4 - PB5
Encoder enc_r(TIM4); // PD12 - PD13

PwmOut motor_l(PB_15);
PwmOut motor_r(PB_13);

DigitalOut signalExperience(PC_8);

DigitalOut direction_r(PF_13);
DigitalOut break_r(PE_9);
DigitalOut direction_l(PE_11);
DigitalOut break_l(PF_14);

DigitalIn starterBtn1(PF_1);
DigitalOut starterBtn2(PF_0);
DigitalIn sideBtn(PC_8);

Ticker updatePos_t;
Ticker checkGP2_t;
Ticker interrupt_nav_update_t;

Position pos(&enc_l, &enc_r);
Navigateur nav(&pos, &motor_l, &motor_r, &direction_l, &direction_r, &enc_l,
               &enc_r);

// // TODO : Donner des ports aux GP2
AnalogIn gp2_analog_1(PA_3); // Devant
AnalogIn gp2_analog_2(PC_0); // Derrière
AnalogIn gp2_analog_3(PC_3); // Droite
AnalogIn gp2_analog_4(PF_3); // Gauche

GP2 gp2_list[4] = {
    GP2(&gp2_analog_1, 0.4), // Devant
    GP2(&gp2_analog_2, 0.4), // Derrière
    GP2(&gp2_analog_3, 0.4), // Droite
    GP2(&gp2_analog_4, 0.5), // Gauche
};

enum Side { BLUE_LEFT, BLUE_RIGHT };

/* Prototypes */

void handleInput();
void updatePos();
void test_motors();
void test_rotation();
void check_all_GP2();
void frein();
void interrupt_nav_update(void);
void send_signal_exp(void);
/* Global variables */

int debug_monitor = 1;
int running = 1;
int move = 1;
enum Side side;

void setup() {

  // Communication série
  usb.baud(115200);
  printf("\n\nInitialisation du programme.\n");

  usb.attach(&handleInput);
  updatePos_t.attach(&updatePos, 0.001f);
  checkGP2_t.attach(&check_all_GP2, 0.1f);
  interrupt_nav_update_t.attach(&interrupt_nav_update, 0.01f);

  starterBtn2 = 0;
  starterBtn1.mode(PullUp);
  sideBtn.mode(PullUp);

  if (sideBtn.read()) {
    side = BLUE_LEFT;
    printf("Coté détecté : Bleu à gauche.\n");
  } else {
    side = BLUE_RIGHT;
    printf("Coté détecté : Bleu à droite.\n");
  }
  nav.set_destination(0, 0);

  printf("Je suis prêt\n");
}

void loop() {

  // ===== MAIN CODE =====

  printf("Attente du démarrage...\n");


  int starterCompteur = 0;
  while (starterCompteur < 100) {
    motor_l.write(0.0f);
    motor_r.write(0.0f);
    nav.reset_pids();
    if (starterBtn1.read() == 0) {
      starterCompteur++;
    } else {
      starterCompteur = 0;
    }
  }

  starterCompteur = 0;
  while (starterCompteur < 100) {
    if (starterBtn1.read()) {
      starterCompteur++;
    } else {
      starterCompteur = 0;
    }
  }
  send_signal_exp();
  frein();
  nav.start_time = nav.time.read_ms();
  printf("C'est parti !\n");
  nav.actionFinished = 0;
  int tmp = 0;
  // while (1)
  // {
  //   nav.go_to(0.5f, 0.0f);
  //   frein();
  // }
  
  if (side == BLUE_LEFT) { // Bleu à gauche

    printf("Action 1\n");
    nav.go_to(0.08f, 0.0f);
    frein();
    printf("Action 2\n");
    nav.rotate_by(PI_OVER_TWO);
    frein();
    printf("Action 3\n");
    nav.go_to(0.08f, 0.55f);
    frein();
    printf("Action 4\n");
    nav.rotate_by(-PI_OVER_TWO);
    frein();
    printf("Action 5\n");
    nav.go_to(0.5f, 0.66f);
    frein();
    printf("Action 6\n");
    nav.rotate_by(-3 * PI / 4 + 0.05);
    frein();
    printf("Action 7\n");
    nav.go_to(0.2f, -0.1f);
    frein();
    printf("Action 8\n");
    nav.rotate_to(-PI);
    frein();
    // fin de la capture des 3 electrons devant les zones
    frein();
    printf("Action 9\n");
    nav.go_to(1.2f - ROBOT_W / 2, -0.3f);
    frein();
    printf("Action 10\n");
    nav.rotate_to(PI_OVER_TWO);
    frein();
    printf("Action 11\n");
    nav.go_to(1.2f - ROBOT_W / 2, 0.35f);
    frein();
    printf("Action 12\n");
    nav.go_to(1.0f - ROBOT_W / 2, 0.3f); // on the zone 
    frein();
    nav.rotate_to(-PI + PI/4);
    frein();
    nav.go_to(0.15f, -0.15f);
    frein();
    while (1) {
    }
  } else {
    printf("Action 1\n");
    nav.go_to(0.08f, 0.0f);
    frein();
    printf("Action 2\n");
    nav.rotate_by(-PI_OVER_TWO);
    frein();
    printf("Action 2\n");
    nav.go_to(0.08f, -0.55f);
    frein();
    nav.rotate_by(PI_OVER_TWO);
    frein();
    nav.go_to(0.5f, -0.55f);
    frein();
    nav.rotate_by(3 * PI / 4 - 0.05);
    frein();
    nav.go_to(0.2f, 0.1f);
    frein();
    nav.rotate_to(PI);
    // fin de la capture des 3 electrons devant les zones
    frein();
    nav.go_to(1.2f - ROBOT_W / 2, 0.3f);
    frein();
    nav.rotate_to(-PI_OVER_TWO);
    frein();
    nav.go_to(1.2f - ROBOT_W / 2, -0.35f);
    frein();
    nav.go_to(1.0f - ROBOT_W / 2, -0.3f);
    frein();
    nav.rotate_to(PI - PI/4);
    frein();
    nav.go_to(0.15f, 0.15f);
    frein();
    while (1) {
    }
  }

  while (1) {
    frein();
  }
  // ===== TESTS =====

  // nav.go_to(1, 0);
  // wait(0.5);
  // nav.rotate_by(PI_OVER_TWO);
  // wait(0.5);
  // nav.rotate_by(-PI);

  // ==========  CARRÉ  ==============

  // while (1) {
  //   if (running) {

  //     nav.avancer(0.3);
  //     nav.rotate_by(PI_OVER_TWO);
  //   }
  // }

  //
  // ======== PID VITESSE ==========
  //
  //  direction_l = 1;
  // direction_r = 0;

  // nav.pid_v_l.reset();
  // nav.pid_v_r.reset();
  // nav.pid_v_l.setCommande(0.1f);
  // nav.pid_v_r.setCommande(0.1f);

  // printf("DEBUT\n");

  // int counter = 0;
  // while (counter < 1000 && running) {
  //   float cml = nav.pid_v_l.getConsigne();
  //   float cmr = nav.pid_v_r.getConsigne();

  //   printf("vl:%f\n", nav.pid_v_l.vitesse);

  //   counter++;
  //   printf("%f %f\n", cmr, cml);

  //   motor_l.write(cml);
  //   motor_r.write(cmr);
  // }

  // printf("FIN_TEST\n");

  // motor_l.write(0.0f);
  // motor_r.write(0.0f);
}

int main() {
  setup();
  loop();
  return 0;
}

void test_motors() {

  // printf("Avancer\n");
  // // Avancer tout droit
  // motor_l.write(0.1f);
  // direction_l = 1;
  // motor_r.write(0.1f);
  // direction_r = 0;

  // wait(1);

  // // motor_l.write(0.0f);
  // // motor_r.write(0.0f);

  // printf("Reculer\n");
  // // wait(2);

  // // Reculer tout droit
  // // motor_l.write(0.2f);
  // direction_l = 0;
  // // motor_r.write(0.2f);
  // direction_r = 1;

  // wait(1);

  // motor_l.write(0.0f);
  // motor_r.write(0.0f);

  // wait(1);

  // // Tourner
  // motor_l.write(0.2f);
  // direction_l = 1;
  // motor_r.write(0.2f);
  // direction_r = 1;

  // wait(1);

  // // Tourner
  // direction_l = 0;
  // direction_r = 0;

  // wait(1);

  // motor_l.write(0.0f);
  // motor_r.write(0.0f);

  // // tourner
  direction_l = 1;
  direction_r = 0;

  nav.pid_v_l.reset();
  nav.pid_v_r.reset();
  nav.pid_v_l.setCommande(0.1f);
  nav.pid_v_r.setCommande(0.1f);

  printf("DEBUT\n");

  int counter = 0;
  while (counter < 100) {
    float cml = nav.pid_v_l.getConsigne();
    float cmr = nav.pid_v_r.getConsigne();

    printf("vl:%f\n", nav.pid_v_l.vitesse);

    counter++;
    printf("%f %f\n", cmr, cml);

    motor_l.write(cml);
    motor_r.write(cmr);
  }

  printf("FIN_TEST\n");

  motor_l.write(0.0f);
  motor_r.write(0.0f);
}

void handleInput() {
  char c = usb.getc();

  if (c == 'q') {
    running = running ? 0 : 1;
    if (running)
      printf("\nComputation activé.\n");
    else
      printf("\nComputation désactivé.\n");
    if (!running) {
      motor_l.write(0.0f);
      motor_r.write(0.0f);
    }
  }

  if (c == 'm') {
    move = move ? 0 : 1;
    if (move)
      printf("\nMouvement activé.\n");
    else
      printf("\nMouvement désactivé.\n");
    if (!move) {
      frein();
    }
  }

  if (c == 's') { // set destination
    printf("\nSet mode, chose a destination: \n");

    float x;
    printf("\nx = ");
    usb.scanf("%f", &x);

    float y;
    printf("\ny = ");
    usb.scanf("%f", &y);

    Vecteur2D destination(x, y);
    nav.set_destination(x, y);

    printf("\nNew destination set : (%f, %f)\n", x, y);
  }

  if (c == 'd') { // Toggle debug
    debug_monitor = debug_monitor ? 0 : 1;
    if (debug_monitor)
      printf("\nDebug activé.\n");
    else
      printf("\nDebug désactivé.\n");
  }

  if (c == 'r') { // reset
    printf("\nResetting the program.\n");
    SCB->AIRCR = 0x05fa0004;
    // reset();
    running = 1;
    setup();
  }

  if (c == 'p') { // Print position
    nav.print_pos();
  }

  if (c == 't') { // Run Test
    test_motors();
  }

  if (c == 'f') {
    frein();
  }
}

void test_rotation() {
  nav.pid_a.setCommande(-PI_OVER_TWO);

  while (running) {
    float command = nav.pid_a.getConsigne();

    int d_l = 0;
    int d_r = 1;

    nav.pid_v_l.setCommande(-command);
    nav.pid_v_r.setCommande(command);

    float cml = nav.pid_v_l.getConsigne();
    float cmr = nav.pid_v_r.getConsigne();

    limiter_consigne(&cml, &d_l);
    limiter_consigne(&cmr, &d_r);

    nav.print_pos();
    printf("%f %f %f ", command, cml, cmr);

    if (move) {
      motor_l.write(cml);
      motor_r.write(cmr);
    }

    direction_l = d_l;
    direction_r = d_r;
  }
}

void updatePos() { nav.updatePos(); }

/* Return whether the position might be a wall */
int pos_is_a_wall(float x, float y) {
  return (abs(x - (0 - X_INIT)) < WALL_DETECTION_GAP) ||
         (abs(x - (HEIGHT_TABLE - X_INIT)) < WALL_DETECTION_GAP) ||
         (abs(x - (0 - Y_INIT)) < WALL_DETECTION_GAP) ||
         (abs(x - (WIDTH_TABLE - X_INIT)) < WALL_DETECTION_GAP) ||
         (abs(x - (CENTER_WALL_POS - X_INIT)) < WALL_DETECTION_GAP);
}

/* Return whether the position (x, y) is on the table */
int pos_is_on_table(float x, float y) {
  return (x < HEIGHT_TABLE - X_INIT) && (x > 0 - X_INIT) &&
         (y < WIDTH_TABLE - Y_INIT) && (y > 0 - Y_INIT);
}

void check_all_GP2() {
  if (int num = detected_all(gp2_list, 4)) {
    float theta = pos.get_theta();
    float obj_x;
    float obj_y;
    printf("%d\n", num);
    switch (num) {
    case 1: // Front GP2
      if (nav.sens == AVANT) {
        obj_x = pos.get_x() + gp2_list[num - 1].real_distance * cos(theta);
        obj_y = pos.get_y() + gp2_list[num - 1].real_distance * sin(theta);
        if (pos_is_on_table(obj_x, obj_y) && !pos_is_a_wall(obj_x, obj_y)) {
          frein();
          if (debug_monitor)
            printf("\nStopped because object on the table\n");
        };
      }
      break;
    case 2: // Back GP2
      if (nav.sens == ARRIERE) {
        obj_x = pos.get_x() - gp2_list[num - 1].real_distance * cos(theta);
        obj_y = pos.get_y() - gp2_list[num - 1].real_distance * sin(theta);
        if (pos_is_on_table(obj_x, obj_y) && !pos_is_a_wall(obj_x, obj_y)) {
          frein();
          if (debug_monitor)
            printf("\nObject behind on the table\n");
        };
      }
      break;
    case 3: // Right
      if (nav.sens == AVANT) {
        obj_x = pos.get_x() + gp2_list[num - 1].real_distance * cos(theta);
        obj_y = pos.get_y() + gp2_list[num - 1].real_distance * sin(theta);
        if (pos_is_on_table(obj_x, obj_y) && !pos_is_a_wall(obj_x, obj_y)) {
          frein();
          if (debug_monitor)
            printf("\nObject right on the table\n");
        };
      }
      break;
    case 4: // Left
      if (nav.sens == AVANT) {
        obj_x = pos.get_x() + gp2_list[num - 1].real_distance * cos(theta);
        obj_y = pos.get_y() + gp2_list[num - 1].real_distance * sin(theta);
        if (pos_is_on_table(obj_x, obj_y) && !pos_is_a_wall(obj_x, obj_y)) {
          frein();
          if (debug_monitor)
            printf("\nObject left on the table\n");
        };
        break;
      }
    default:
      break;
    }
  }
}

void frein() {
  motor_l.write(0.0f);
  motor_r.write(0.0f);
  nav.reset_pids();
  wait(0.1f);
}

void interrupt_nav_update(void) { nav.update(); }

void send_signal_exp(){
  for(int i = 0 ; i< 1000 ; i++){
    signalExperience = 1;
    wait_us(300);
    signalExperience = 0;
    wait_us(200);
  }
}

