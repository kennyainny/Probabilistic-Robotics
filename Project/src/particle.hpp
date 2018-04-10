#ifndef PARTICLE_H
#define PARTICLE_H

#include "def_type.hpp"
#include "motion.hpp"
#include "sensor.hpp"

void new_hornetsoft_particle(particle_type *particle, long unsigned int size_p);
void initialize_particle(particle_type *particle, Vector3d p1, Vector3d p2, Vector3d p3);
particle_type particle_motion_update(particle_type p_particle, Vector3d p1, Vector3d p2, Vector3d p3);
particle_type particle_sensor_update(particle_type particle, Vector3d p1, Vector3d p2, Vector3d p3);
particle_type low_variance_sampler(particle_type particle);
state_type expected_state(particle_type particle);

#endif /* PARTICLE_H */