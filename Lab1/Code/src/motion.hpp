#ifndef MOTION_H
#define MOTION_H

extern "C"
{
	#include "def_type.h"
}

state_type sample_motion_model_odometry(odometry_type p_odometry, odometry_type odometry, state_type p_state);

#endif /* MOTION_H */