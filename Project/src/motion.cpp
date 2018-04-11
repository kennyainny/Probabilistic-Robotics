#include "motion.hpp"

#define SIGMA_R 0.03
#define SIGMA_THETA RAD(45)

using namespace std;
random_device rd_m;
mt19937 mt_m(rd_m());

state_type sample_motion_model(state_type p_state, Vector3d p1, Vector3d p2, Vector3d p3)
{
	state_type state;
	float r, theta;
	normal_distribution<float> distr;

	distr.param(normal_distribution<float>::param_type(0.0, SIGMA_R));
	r = distr(mt_m);

	distr.param(normal_distribution<float>::param_type(0.0, SIGMA_THETA));
	theta = distr(mt_m);

	state.x = p_state.x + r*cos(p_state.theta + theta);
	state.y = p_state.y + r*sin(p_state.theta + theta);
	state.theta = p_state.theta + theta;

	state = motion_constraint(state, p_state, p1, p2, p3); //check if the particles are in the UAVs' eye sight

	return state;
}

state_type motion_constraint(state_type state, state_type p_state, Vector3d p1, Vector3d p2, Vector3d p3){
	state_type state_temp;
	float z1, z2, z3, theta1, theta2, theta3;
	z1 = sqrt(pow(state.x - p1(0), 2) + pow(state.y - p1(1), 2));
	z2 = sqrt(pow(state.x - p2(0), 2) + pow(state.y - p2(1), 2));
	z3 = sqrt(pow(state.x - p3(0), 2) + pow(state.y - p3(1), 2));

	theta1 = atan2(state.y - p1(1), state.x - p1(0));
	if(theta1 < RAD(0)) theta1 += RAD(360);
	theta1 = theta1 - p1(2) + RAD(SENSOR_VIEW/2);

	theta2 = atan2(state.y - p2(1), state.x - p2(0));
	if(theta2 < RAD(0)) theta2 += RAD(360);
	theta2 = theta2 - p2(2) + RAD(SENSOR_VIEW/2);

	theta3 = atan2(state.y - p3(1), state.x - p3(0));
	if(theta3 < RAD(0)) theta3 += RAD(360);
	theta3 = theta3 - p3(2) + RAD(SENSOR_VIEW/2);

	if(z1 <= MAX_RANGE){
		if(theta1 >= RAD(0) && theta1 <= RAD(SENSOR_VIEW)){
			state_temp = state;
		}else{
			state_temp = p_state;
		}
	}else if(z2 <= MAX_RANGE){
		if(theta2 >= RAD(0) && theta2 <= RAD(SENSOR_VIEW)){
			state_temp = state;
		}else{
			state_temp = p_state;
		}
	}else if(z3 <= MAX_RANGE){
		if(theta3 >= RAD(0) && theta3 <= RAD(SENSOR_VIEW)){
			state_temp = state;
		}else{
			state_temp = p_state;
		}
	}else{
		state_temp = p_state;
	}

	return state_temp;
}