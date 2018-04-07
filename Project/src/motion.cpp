#include "motion.hpp"
#include <iostream>
#include <random>
#include "math.h"

using namespace std;
random_device rd_m;
mt19937 mt_m(rd_m());

state_type sample_motion_model_odometry(state_type p_odometry, state_type odometry, state_type p_state)
{
	state_type state;
	float rot1, rot2, trans, _rot1, _rot2, _trans;
	float alpha[4] = {0.005, 0.005, 0.01, 0.01}; //still need to be adjusted

	normal_distribution<float> distr;

	rot1 = atan2(odometry.y - p_odometry.y, odometry.x - p_odometry.x) - p_odometry.theta;
	trans = sqrt(pow((odometry.x - p_odometry.x), 2) + pow((odometry.y - p_odometry.y), 2));
	rot2 = odometry.theta - p_odometry.theta - rot1;

	// distr.param(normal_distribution<float>::param_type(0.0, alpha[0]*pow(rot1, 2) + alpha[1]*pow(trans, 2)));
	// _rot1 = rot1 - distr(mt_m);

	// distr.param(normal_distribution<float>::param_type(0.0, alpha[2]*pow(trans, 2) + alpha[3]*pow(rot1, 2) + alpha[3]*pow(rot2, 2)));
	// _trans = trans - distr(mt_m);

	// distr.param(normal_distribution<float>::param_type(0.0, alpha[0]*pow(rot2, 2) + alpha[1]*pow(trans, 2)));
	// _rot2 = rot2 - distr(mt_m);

	distr.param(normal_distribution<float>::param_type(0.0, alpha[0]*rot1 + alpha[1]*trans));
	_rot1 = rot1 - distr(mt_m);

	distr.param(normal_distribution<float>::param_type(0.0, alpha[2]*trans + alpha[3]*(rot1 + rot2)));
	_trans = trans - distr(mt_m);

	distr.param(normal_distribution<float>::param_type(0.0, alpha[0]*rot2 + alpha[1]*trans));
	_rot2 = rot2 - distr(mt_m);

	state.x = p_state.x + _trans*cos(p_state.theta + _rot1);
	state.y = p_state.y + _trans*sin(p_state.theta + _rot1);
	state.theta = p_state.theta + _rot1 + _rot2;

	return state;
}