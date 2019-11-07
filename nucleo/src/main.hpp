#ifndef MAIN_HPP
#define MAIN_HPP


// LED
#define PERIOD_LED 1
#define LED_R_PIN PB_13
#define LED_G_PIN PB_14
#define LED_B_PIN PB_15

// 7 SEG
#define SEG_RX PD_2
#define SEG_TX PC_12

// WAITING KEY
#define KEY_PIN PA_0

// SIDE SWITCH
#define SIDE_PIN PC_13

// Left qei
#define ENCODER_TIM_LEFT TIM3
#define GREEN_L PC_7
#define YELLOW_L PC_6
// Right qei
#define ENCODER_TIM_RIGHT TIM4
#define YELLOW_R PB_7
#define GREEN_R PB_6

// Motors
#define PERIOD_PWM 0.000033f
// Left motor
#define PWM_L PB_3
#define DIR_L PA_8
#define BREAK_L PA_10
#define DIR_FWD_L 1
// Right motor
#define PWM_R PB_10
#define DIR_R PA_9
#define BREAK_R PB_5
#define DIR_FWD_R 1

// Pump
#define PUMP_PIN PC_9
#define EVALVE_PIN PB_9

// PID
#define NB_COEF_ERR 4
#define NB_COEF_CO 3

// GP2
#define GP2_FL PA_1
#define GP2_FC PA_4
#define GP2_FR PB_0
#define GP2_RL PC_1
#define GP2_RC PC_0
#define GP2_RR PC_2


#endif
