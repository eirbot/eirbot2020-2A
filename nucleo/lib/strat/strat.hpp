#ifndef STRAT_HPP
#define STRAT_HPP

#include "mbed.h"
#include <rgb.hpp>
#include <gp2.hpp>
#include <servo.hpp>
#include <pump.hpp>
#include <navigator.hpp>
#include <waypoint.hpp>

#define VIOLET 0
#define YELLOW 1

#define DEBOUNCE_MAX 4096

class Strat {
public:
	Strat(Navigator* const _nav, Odometry* const _odometry, Serial* const _seg,
			RGB* const _rgb);
	~Strat();
	void reset();
	void printSeg();
	void init(DigitalIn* const side_switch, DigitalIn* const waiting_key);
	bool run();
private:
	Navigator* const nav;
	Odometry* const odometry;
	Serial* const seg;
	RGB* const rgb;
	Waypoint* wp;
	Timer t;
	Ticker ticker;
	float t_wp;
	int points;
	bool side;
};

extern Waypoint wp_00a;
extern int wp_00a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_00z;
extern int wp_00z_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);

extern Waypoint wp_10a;
extern int wp_10a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_11a;
extern int wp_11a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_12a;
extern int wp_12a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_13a;
extern int wp_13a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_14a;
extern int wp_14a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_15a;
extern int wp_15a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);

extern Waypoint wp_20a;
extern int wp_20a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_21a;
extern int wp_21a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_22a;
extern int wp_22a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_23a;
extern int wp_23a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_24a;
extern int wp_24a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_25a;
extern int wp_25a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_26a;
extern int wp_26a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);

extern Waypoint wp_30a;
extern int wp_30a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_31a;
extern int wp_31a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_32a;
extern int wp_32a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_33a;
extern int wp_33a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);

extern Waypoint wp_40a;
extern int wp_40a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_41a;
extern int wp_41a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_42a;
extern int wp_42a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_43a;
extern int wp_43a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);

extern Waypoint wp_50a;
extern int wp_50a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_51a;
extern int wp_51a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);
extern Waypoint wp_52a;
extern int wp_52a_action(Waypoint** wp, Navigator* nav, Timer* t, float* t_wp);

#endif

