/*
 * TODO
 * Documentation
 */

#include <robot.hpp>

Robot::Robot(Navigator *const _nav, Odometry *const _odometry,
			 Serial *const _seg, RGB *const _rgb, Ctfcom *const _com) : nav(_nav),
																		odometry(_odometry),
																		seg(_seg),
																		rgb(_rgb),
																		com(_com)
{
	reset();
}

Robot::~Robot()
{
}

void Robot::reset()
{
	side = false;
	rgb->off();
	nav->reset();
	t.stop();
	t.reset();
}

void Robot::printScore(int score)
{
	seg->printf("$%3.3d", score);
}

void Robot::init(DigitalIn *const side_switch, DigitalIn *const waiting_key)
{
	rgb->setColor(1, 0, 0);
	uint32_t debounce_counter = 0;
	while (debounce_counter < DEBOUNCE_MAX)
	{
		debounce_counter += !waiting_key->read(); // FIXME PULL up/down ?
	}
	debounce_counter = 0;
	while (debounce_counter < DEBOUNCE_MAX)
	{
		if (side_switch->read() == PURPLE)
		{
			rgb->setColor(1, 0, 1);
		}
		else if (side_switch->read() == YELLOW)
		{
			rgb->setColor(1, 1, 0);
		}
		else
		{
			rgb->setColor(1, 0, 0);
		}
		debounce_counter += waiting_key->read();
	}
	side = side_switch->read();
	if (side == PURPLE)
	{
		// TODO NOTIFY RASP
	}
	else
	{
		// TODO NOTIFY RASP
	}
	nav->start();
	t.start();
}

bool Robot::run()
{
	// TODO notify rasp we have started ?
	while (1)
	{
		serial_answer();
	}
}

void Robot::serial_answer(void)
{
	uint8_t recived_order;
	uint8_t frame_recived_is_known = 1;
	size_t frames_recived = com->get_len_recived_data();
	float x, y, angle;
	if (frames_recived >= 1)
	{
		recived_order = com->get_recived_data();
		if (recived_order == get_pos)
		{
			odometry->getPos(&x, &y, &angle);
			com->answer_get_pos(x, y, angle);
		}
		else if (recived_order == set_pos)
		{
			//while (com.get_len_recived_data() < 12){}
			com->answer_set_pos(&x, &y, &angle);
			odometry->setPos(x, y, angle);
		}
		else if (recived_order == go_to)
		{
			com->decode(&x, &y, &angle);
			nav->setDst(x, y, angle);
		}
		else if (recived_order == stop)
		{
			// Stop the robot
		}
		else if (recived_order == panic)
		{
			// The rpi is dead, go to the end
		}
		else if (recived_order == get_gp2)
		{
			// com->generic_send(get_gp2,OUI);
		}
		else
		{
			frame_recived_is_known = 0;
		}

		if (frame_recived_is_known)
		{
			com->generic_send(ack);
		}
		else
		{
			com->generic_send(nack);
			// traiter l'erreur
		}
	}
}
