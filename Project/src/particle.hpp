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
particle_type particle_motion_update(state_type p_odometry, state_type odometry, particle_type p_particle, map_type map);
particle_type particle_sensor_update(laser_type laser, map_type map, particle_type particle);
particle_type low_variance_sampler(laser_type laser, map_type map, particle_type particle);
state_type expected_state(particle_type particle);

#endif /* PARTICLE_H */