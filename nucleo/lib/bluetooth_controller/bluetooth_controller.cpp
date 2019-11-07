
#include "bluetooth_controller.hpp"

Serial bt(BT_TX, BT_RX);

int mode = 0; // 0=>receive    1=>traitement

int keyNumber = 0;
int key[5];

int LX = 127, LY = 127, R2=0;
float speedLeft = 0.0f, speedRight = 0.0f, multi = 1.0f;

void initBtPS3Controller(SpeedBlock *speed) {

	speed->reset();
	speed->start();
	bt.baud(9600);

}

void bluetooth_controller_PS3(RGB *led, SpeedBlock *speed, DigitalOut *ev, DigitalOut *po, DigitalOut *poki) {

	initBtPS3Controller(speed);
	int statePompe = 1;

	while (1) {

		if (mode == 0) {
			if (bt.readable()) {

				key[keyNumber] = bt.getc();

				if ((key[keyNumber] == '\n') || (keyNumber == 5)) {
					mode = 1;
					keyNumber = 0;
				} else
					keyNumber++;
			}
		} else if (mode == 1) {

			// Bouton croix
			if (key[0] == 'B' && key[1] == 'X') {
				if (key[2] == '0') {
					poki->write(0);
				} else if (key[2] == '1') {
					poki->write(1);
				}
			}

			// Bouton carré
			if (key[0] == 'B' && key[1] == '#') {
				if (key[2] == '0') {
					servoSetPwmDuty(SERVO_INIT);
				} else if (key[2] == '1') {
					servoSetPwmDuty(SERVO_RIGHT);
				}
			}

			// Bouton rond
			if (key[0] == 'B' && key[1] == 'O') {
				if (key[2] == '0') {
					ev->write(1);
				} else if (key[2] == '1') {
					statePompe = 1;
					ev->write(0);
					po->write(statePompe);
				}
			}

			// Bouton triangle
			if (key[0] == 'B' && key[1] == 'V') {
				if (key[2] == '0') {

				} else if (key[2] == '1') {

					statePompe = ((statePompe == 1) ? 0 : 1);
					po->write(statePompe);
				}
			}

			// joystick gauche axe X
			if (key[0] == 'L' && key[1] == 'X') {
				LX = key[2];
			}

			// joystick gauche axe X
			if (key[0] == 'L' && key[1] == 'Y') {
				LY = key[2];
			}

			// gachette R1
			if (key[0] == 'B' && key[1] == 'R') {
				if (key[2] == '0') {
					multi = 1.0f;
				} else if (key[2] == '1') {
					multi = 2.0f;
				}
			}

			speedLeft = (-(((float) LY - 127.0f) / 128.0f) + (((float) LX - 127.0f) / 128.0f)) * 50.0f * multi;
			speedRight = (-(((float) LY - 127.0f) / 128.0f) - (((float) LX - 127.0f) / 128.0f)) * 50.0f * multi;
			//printf("speedLeft = %f\tspeedRight = %f\t LX = %d\t LY = %d\n",speedLeft, speedRight, LX, LY);
			speed->setSpeed(speedLeft, speedRight); //60 max c'est bien

			mode = 0;
		}


	}
}