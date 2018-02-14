#include "main.hpp"
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <algorithm>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
	printf("Hello, Georgia Tech! \n");

	/* Initialization Part*/
	map_type map;
	read_beesoft_map(argv[1], &map); //store map data in map variable

	sensor_type sensor;
	read_beesoft_sensor(argv[2], &sensor); //store sensor data in sensor variable
	printf("done1\n");
	particle_type particle[sensor.odometry_count + 1];
	particle_initialize(map, &particle[0]); //initialize particle
	printf("done2\n");
	state_type filtered_state[sensor.odometry_count + 1];
	filtered_state[0] = expected_state(particle[0]);
	intrinsic_param_type param;
// <<<<<<< HEAD
	//float *z;//[particle[0].particle_count];
	
	// calc_z_star_array(particle[0],*z, map);
	// printf("z star value\n");
	// printf("%f\n", z[0]);
	// printf("Intrinsic_parameters\n")
	// intrinsic_parameters(particle[0].state[0], map, sensor, &param,particle[0]); //optimize sensor param
	// printf("done3\n");
	/* End Initialization */
// =======
// e>>>>>>> 302d4c3bda53df0559d42e21ee5e3f87f62bd8a4

	///////////////////////////////////////////////////////////////////////

	printf("Starting Loop...\n");
	odometry_type odometry;
	odometry.ts = 0;
	odometry.v.x = 0;
	odometry.v.y = 0;
	odometry.v.theta = 0;
	long unsigned int j = 0;
	// sensor.odometry_count = 300;
	for(long unsigned int i = 0; i < sensor.odometry_count; i++){ //define convergent criteria later
		if(i == 0){
			particle[i+1] = particle_filter(particle[i], sensor.laser[j], sensor.odometry[i], sensor.odometry[i], map, param);
		}else{
			if(j < sensor.laser_count && sensor.laser[j+1].ts < sensor.odometry[i].ts){
				j++;
			}
			particle[i+1] = particle_filter(particle[i], sensor.laser[j], sensor.odometry[i-1], sensor.odometry[i], map, param);
		}
		filtered_state[i+1] = expected_state(particle[i+1]);

		// printf("%f %f %f\n", filtered_state[i].x, filtered_state[i].y, filtered_state[i].theta);
		// printf("%f %f %f\n", filtered_state[i+1].x, filtered_state[i+1].y, filtered_state[i+1].theta);
		printf("end step %lu of %lu\n",i+1, sensor.odometry_count);
	}

	prob_visualize(map, particle, filtered_state, sensor.odometry_count);

	waitKey(0);   
	return 0;
}