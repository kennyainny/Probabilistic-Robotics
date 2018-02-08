#include "main.hpp"
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
	printf("Hello, Georgia Tech! \n");

	map_type map;
	read_beesoft_map(argv[1], &map); //store map data in map variable
// 	// prob_visualize(map);

 	sensor_type sensor;
	read_beesoft_sensor(argv[2], &sensor); //store sensor data in sensor variable

	state_type state;
	z_ks(map, state);
	sensor_model(map);

	particle_type particle;
	particle_initialize(map, &particle);

	//Loop should be started here
	particle_filter(particle, sensor.laser[0], sensor.odometry[0], sensor.odometry[1]);

	// for(int i = 1; i < 10; i++){
	// 	sample_motion_model_odometry(sensor.odometry[0], sensor.odometry[1], particle.state[0]);
	// }

	waitKey(0);   
	return 0;
}