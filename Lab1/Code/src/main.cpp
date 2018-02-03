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
	prob_visualize(map);

 	sensor_type sensor;
	read_beesoft_sensor(argv[2], &sensor); //store sensor data in sensor variable

	state_type p_state, state;
	state = sample_motion_model_odometry(sensor.odometry[0], sensor.odometry[1], p_state); // must be called inside particle_filter

	particle_filter(p_state, sensor.laser[0]);

	waitKey(0);   
	return 0;
}