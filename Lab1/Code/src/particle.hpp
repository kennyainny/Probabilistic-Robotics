#ifndef PARTICLE_H
#define PARTICLE_H

extern "C"
{
	#include "def_type.h"
	#include "sensor.h"
}

#include "motion.hpp"

state_type particle_filter(state_type p_state, laser_type laser);

#endif /* PARTICLE_H */