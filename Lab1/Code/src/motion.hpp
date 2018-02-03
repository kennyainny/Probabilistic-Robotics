#ifndef MOTION_H
#define MOTION_H

extern "C"
{
  #include "sensor.h"
}

#include "main.hpp"

state_type sample_motion_model_odometry(odometry_type p_odometry, odometry_type odometry, state_type p_state);

#endif /* MOTION_H */