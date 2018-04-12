#ifndef MOTION_H
#define MOTION_H

#include "def_type.hpp"
#include "sensor.hpp"
#include <iostream>
#include <random>
#include "math.h"

state_type sample_motion_model(state_type p_state, Vector3d p1, Vector3d p2, Vector3d p3);
state_type motion_constraint(state_type state, state_type p_state, Vector3d p1, Vector3d p2, Vector3d p3);

#endif /* MOTION_H */