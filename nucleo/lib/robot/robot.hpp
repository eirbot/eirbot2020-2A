#ifndef STRAT_HPP
#define STRAT_HPP

#include "mbed.h"
#include <rgb.hpp>
#include <gp2.hpp>
#include <servo.hpp>
#include <navigator.hpp>
#include <waypoint.hpp>
#include <ctfcom.h>

#define PURPLE 0
#define YELLOW 1

#define DEBOUNCE_MAX 4096

class Robot {
public:
	Robot(Navigator* const _nav, Odometry* const _odometry, Serial* const _seg,
			RGB* const _rgb, Ctfcom* const _com);
	~Robot();
	void reset();
	void printScore(int score);
	void init(DigitalIn* const side_switch, DigitalIn* const waiting_key);
	bool run();
private:
	Navigator* const nav;
	Odometry* const odometry;
	Serial* const seg;
	RGB* const rgb;
	Ctfcom* const com;
	Timer t;
	Ticker ticker;
	bool side;
	void serial_answer(void);
};

#endif

