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

	map_type map;
	read_beesoft_map(argv[1], &map); //store map data in map variable
	prob_visualize(map);

 	sensor_type sensor;
	read_beesoft_sensor(argv[2], &sensor); //store sensor data in sensor variable

	// float laser_max1 = 0, laser_max2 = 0, laser_max3 = 0;;
	// for(int i = 0; i < sensor.laser_count; i++){
	// 	for(int j = 0; j < 180; j++){
	// 		laser_max2 = max(laser_max2, sensor.laser[i].r[j-1]);
	// 	}
	// 	laser_max1 = max(laser_max2,laser_max1);
	// }
	// printf("\n: %f : \n", laser_max1);

	// state_type state;
	// z_ks(map, state);
	// sensor_model(map);

	particle_type particle;
	particle_initialize(map, &particle);
	printf("%f\n", particle.state[1].x);
	printf("%f\n", particle.state[1].y);
	printf("%f\n", particle.state[1].theta);


	//Loop should be started here
	// particle_filter(particle, sensor.laser[0], sensor.odometry[0], sensor.odometry[1]);

	// for(int i = 1; i < 10; i++){
	// 	sample_motion_model_odometry(sensor.odometry[0], sensor.odometry[1], particle.state[0]);
	// }

	waitKey(0);   
	return 0;
}