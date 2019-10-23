#ifndef __CONFIG_H_
#define __CONFIG_H_

#define ENCODER_TIM_LEFT TIM4

#define ENCODER_TIM_RIGHT TIM3

#define PI 3.141592653589793f
#define PI_OVER_TWO 1.57079632679f
// Il y a un terme correctif
#define DIAMETER                                                               \
  (0.247f * 0.9190936949450285f) // largeur en mètres entre les deux encodeurs
                                 // ( ATTENTION : la valeur est arbitraire !)

#define ENCODEUR_ECHELLE 44669.0f // ticks par mètres
// Ancienne valeur 2 : 45703.0f   // ticks par mètres
// Ancienne valeur   : 43723.0f   // ticks par mètres

#define CONSIGNE_MAX 0.4f

/* ===  WALL DETECTION  === */

// TODO : Check table values
#define HEIGHT_TABLE 2.0f
#define WIDTH_TABLE 2.0f
#define X_INIT 0.15f
#define Y_INIT (side == BLUE_RIGHT ? 0.75f : 1.25f)
#define CENTER_WALL_POS (side == BLUE_RIGHT ? 1.5f : 0.5f)

// Error accepted to validate that this is a wall
#define WALL_DETECTION_GAP 0.10f

#define ROBOT_W 0.15f

#endif // __CONFIG_H_
