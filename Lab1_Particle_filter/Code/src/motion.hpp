#ifndef MOTION_H
#define MOTION_H

extern "C"
{
	#include "def_type.h"
}

state_type sample_motion_model_odometry(state_type p_odometry, state_type odometry, state_type p_state, map_type map);

#endif /* MOTION_H */