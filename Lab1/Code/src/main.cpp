#include "main.hpp"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	printf("Hello, Georgia Tech! \n");

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
	int j = -1;
	for (unsigned long int t = 1; t < sensor.sensor_count; t++){

		particle[t] = particle_motion_update(sensor.v[t-1], sensor.v[t], particle[t-1]);
		if (sensor.type[t] == 1){
			j = j + 1;
			particle[t] = particle_sensor_update(sensor.laser[j], map, particle[t]);
			particle[t] = low_variance_sampler(sensor.laser[j], map, particle[t]);
		}

		filtered_state[t] = expected_state(particle[t]);

		printf("aaa\n");
		// particle_visualize(particle[t], filtered_state[t], sensor.laser[j], map);
		printf("end step %lu of %lu\n", t, sensor.sensor_count);
	}

	waitKey(0);   
	return 0;
}