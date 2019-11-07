#include "mbed.h"
#include <rgb.hpp>
#include <servo.hpp>
#include <speed_block.hpp>
#include <odometry.hpp>

#define BT_RX PC_11
#define BT_TX PC_10



void initBtPS3Controller();
void bluetooth_controller_PS3(RGB *led, SpeedBlock *speed, DigitalOut *ev, DigitalOut *po, DigitalOut *poki);