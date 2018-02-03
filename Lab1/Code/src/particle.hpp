#ifndef PARTICLE_H
#define PARTICLE_H

extern "C"
{
	#include "def_type.h"
}

state_type particle_filter(state_type p_state, laser_type laser);

#endif /* PARTICLE_H */