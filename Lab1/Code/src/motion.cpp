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

	rot1 = atan2(odometry.v.y - p_odometry.v.y, odometry.v.x - p_odometry.v.x) - p_odometry.v.theta;
	trans = sqrt(pow((odometry.v.x - p_odometry.v.x), 2) + pow((odometry.v.y - p_odometry.v.y), 2));
	rot2 = odometry.v.theta - p_odometry.v.theta - rot1;

	distr.param(normal_distribution<float>::param_type(0.0, alpha[0]*pow(rot1, 2) + alpha[1]*pow(trans, 2)));
	_rot1 = rot1 - distr(mt);

	distr.param(normal_distribution<float>::param_type(0.0, alpha[2]*pow(trans, 2) + alpha[3]*pow(rot1, 2) + alpha[3]*pow(rot2, 2)));
	_trans = trans - distr(mt);

	distr.param(normal_distribution<float>::param_type(0.0, alpha[0]*pow(rot2, 2) + alpha[1]*pow(trans, 2)));
	_rot2 = rot2 - distr(mt);

	// printf("m1: %f %f %f\n", rot1, trans, rot2);
	// printf("m2: %f %f %f\n", _rot1, _trans, _rot2);

	state.x = p_state.x + _trans*cos(p_state.theta + _rot1);
	state.y = p_state.y + _trans*sin(p_state.theta + _rot1);
	state.theta = p_state.theta + _rot1 + _rot2;
	if(state.theta > M_PI){
		while(state.theta > M_PI){
			state.theta = state.theta - 2*M_PI;
		}
	}else if(state.theta < -M_PI){
		while(state.theta < -M_PI){
			state.theta = state.theta + 2*M_PI;
		}
	}

	return state;
}