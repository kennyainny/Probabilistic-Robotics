#ifndef MOTION_H
#define MOTION_H

#include "def_type.hpp"

state_type sample_motion_model_odometry(state_type p_odometry, state_type odometry, state_type p_state);

#endif /* MOTION_H */