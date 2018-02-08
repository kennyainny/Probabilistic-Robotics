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

<<<<<<< HEAD
 	sensor_type sensor;
=======

 	sensor_type sensor;
	particle_type particle;
	//state_type state;
>>>>>>> 119773e64978b25beeb279f586b42e5e36273cae
	read_beesoft_sensor(argv[2], &sensor); //store sensor data in sensor variable


	particle_initialize(map, &particle);
	printf("%f\n", particle.state[1].x);
	printf("%f\n", particle.state[1].y);
	printf("%f\n", particle.state[1].theta);
	z_ks(map, particle.state[1]);


<<<<<<< HEAD
=======

>>>>>>> 119773e64978b25beeb279f586b42e5e36273cae
	//Loop should be started here
	// particle_filter(particle, sensor.laser[0], sensor.odometry[0], sensor.odometry[1]);

	// for(int i = 1; i < 10; i++){
	// 	sample_motion_model_odometry(sensor.odometry[0], sensor.odometry[1], particle.state[0]);
	// }

	waitKey(0);   
	return 0;
}