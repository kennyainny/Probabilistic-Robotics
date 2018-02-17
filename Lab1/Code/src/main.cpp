#include "main.hpp"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	/****************************** Initialization Part ******************************/
	map_type map;
	read_beesoft_map(argv[1], &map); //store map data in map variable

	sensor_type sensor;
	read_beesoft_sensor(argv[2], &sensor); //store sensor data in sensor variable

	particle_type particle[sensor.sensor_count];
	particle_initialize(map, &particle[0]); //initialize particle

	state_type filtered_state[sensor.sensor_count];
	filtered_state[0] = expected_state(particle[0]); //initialize expected location

	/**************************** Particle Filtering Part ****************************/
	int j = 0;
	for (unsigned long int t = 1; t <= sensor.sensor_count; t++){

		if(t < sensor.sensor_count){
			particle[t] = particle_motion_update(sensor.v[t-1], sensor.v[t], particle[t-1], map); //randomly move particles
			if (sensor.type[t] == 1){			
				particle[t] = particle_sensor_update(sensor.laser[j], map, particle[t]); //update particles' probability
				particle[t] = low_variance_sampler(sensor.laser[j], map, particle[t]); //sampling new set of particles
				if(j < sensor.laser_count)
					j = j + 1;
			}
			filtered_state[t] = expected_state(particle[t]); //average location
		}

		particle_visualize(particle[t-1], filtered_state[t-1], sensor.laser[j], map, t-1); //real-time particles' movement
		printf("Step %lu of %lu\n", t, sensor.sensor_count); //just telling a progess
	}
	while(1){
		/* Does nothing but smiling at you :) */
	};
	return 0;
}