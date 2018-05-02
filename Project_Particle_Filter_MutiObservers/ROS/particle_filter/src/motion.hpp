#ifndef MOTION_H
#define MOTION_H

#include "def_type.hpp"
#include "sensor.hpp"
#include <iostream>
#include <random>
#include "math.h"

float step(float x, float y1, float y2);
float linear(float x, float y1, float y2);
float square(float x, float y1, float y2);
void sigma_mean_calc(state_type state, Vector3d p1, Vector3d p2, Vector3d p3, float *mean_R, float *sigma_R, float *mean_theta, float *sigma_theta);
state_type sample_motion_model(state_type p_state, Vector3d p1, Vector3d p2, Vector3d p3);
state_type motion_constraint(state_type state, state_type p_state, Vector3d p1, Vector3d p2, Vector3d p3);

#endif /* MOTION_H */