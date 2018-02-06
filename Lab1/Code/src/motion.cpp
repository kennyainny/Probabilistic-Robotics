#include "motion.hpp"
#include <iostream>
#include <random>
#include "math.h"

using namespace std;
random_device rd;
mt19937 mt(rd());

state_type sample_motion_model_odometry(odometry_type p_odometry, odometry_type odometry, state_type p_state)
{
	state_type state;
	float rot1, rot2, trans, _rot1, _rot2, _trans;
	float alpha[4] = {1.0, 1.0, 1.0, 1.0}; //need to be adjusted
	normal_distribution<float> distr;

	rot1 = atan2(odometry.v.y - p_odometry.v.y, odometry.v.x - p_odometry.v.x) - RAD(p_odometry.v.theta);
	trans = sqrt(pow((odometry.v.x - p_odometry.v.x), 2) + pow((odometry.v.y - p_odometry.v.y), 2));
	rot2 = RAD(odometry.v.theta) - RAD(p_odometry.v.theta) - rot1;

	distr.param(normal_distribution<float>::param_type(0.0, alpha[0]*pow(rot1, 2) + alpha[1]*pow(trans, 2)));
	_rot1 = rot1 - distr(mt);

	distr.param(normal_distribution<float>::param_type(0.0, alpha[2]*pow(trans, 2) + alpha[3]*pow(rot1, 2) + alpha[3]*pow(rot2, 2)));
	_trans = trans - distr(mt);

	distr.param(normal_distribution<float>::param_type(0.0, alpha[0]*pow(rot2, 2) + alpha[1]*pow(trans, 2)));
	_rot2 = rot2 - distr(mt);

	state.x = p_state.x + _trans*cos(RAD(p_state.theta) + _rot1);
	state.y = p_state.y + _trans*sin(RAD(p_state.theta) + _rot1);
	state.theta = DEG(RAD(p_state.theta) + _rot1 + _rot2);

	return state;
}