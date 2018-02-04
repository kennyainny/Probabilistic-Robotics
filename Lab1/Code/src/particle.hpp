#ifndef PARTICLE_H
#define PARTICLE_H

extern "C"
{
	#include "def_type.h"
	#include "sensor.h"
}

#include "motion.hpp"

void new_hornetsoft_particle(particle_type *particle, long unsigned int size_p);
void particle_initialize(map_type map, particle_type *particle);
particle_type particle_filter(particle_type p_particle, laser_type laser, odometry_type p_odometry, odometry_type odometry);

#endif /* PARTICLE_H */