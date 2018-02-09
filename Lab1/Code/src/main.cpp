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
	// prob_visualize(map);

	sensor_type sensor;
	read_beesoft_sensor(argv[2], &sensor); //store sensor data in sensor variable

<<<<<<< HEAD
	particle_type particle;
	particle_initialize(map, &particle);
	printf("%f\n", particle.state[1].x);
	printf("%f\n", particle.state[1].y);
	printf("%f\n", particle.state[1].theta);
	float z;
	calc_z_star_array(particle,&z,map);
	intrinsic_param_type param;
	intrinsic_parameters(particle.state[1],map,sensor,&param,&z);
	// z_ks(map, particle.state[1]);




	//Loop should be started here
	// particle_filter(particle, sensor.laser[0], sensor.odometry[0], sensor.odometry[1]);

	// for(int i = 1; i < 10; i++){
	// 	sample_motion_model_odometry(sensor.odometry[0], sensor.odometry[1], particle.state[0]);
	// }
=======
	particle_type particle[sensor.odometry_count + 1];
	particle_initialize(map, &particle[0]); //initialize particle
	
	state_type filtered_state[sensor.odometry_count + 1];
	filtered_state[0] = expected_state(particle[0]);

	intrinsic_param_type param;
	intrinsic_parameters_initialize(&param);
	printf("%f\n", param.z_hit[2]);
	// intrinsic_parameters(particle[0].state[1], map, sensor, &param); //optimize sensor param
	/* End Initialization */

	///////////////////////////////////////////////////////////////////////

	/* Loop should be started here */
	printf("Starting Loop...\n");
	odometry_type odometry;
	odometry.ts = 0;
	odometry.v.x = 0;
	odometry.v.y = 0;
	odometry.v.theta = 0;
	int j = 0;
	for(int i = 0; i < sensor.odometry_count; i++){ //define convergent criteria later
		if(i == 0){
			particle[i+1] = particle_filter(particle[i], sensor.laser[j], odometry, sensor.odometry[i], map, param);
		}else{
			if(j < sensor.laser_count && sensor.laser[j+1].ts < sensor.odometry[i].ts){
	// 			j++;
			}
	// 		particle[i+1] = particle_filter(particle[i], sensor.laser[j], sensor.odometry[i-1], sensor.odometry[i], map, param);
		}
		// filtered_state[i+1] = expected_state(particle[i+1]);

		printf("%f %f %f\n", filtered_state[i].x, filtered_state[i].y, filtered_state[i].theta);	
	}
>>>>>>> e6274830aff1420c97e1199b2a3f0eeca7d04fe4

	waitKey(0);   
	return 0;
}