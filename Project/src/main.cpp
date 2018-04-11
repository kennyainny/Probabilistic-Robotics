#include "main.hpp"

using namespace std;
using namespace Eigen;

int main(int argc, char *argv[])
{
	/****************************** Initialization Part ******************************/
	Vector3d p_gt, p_gt_old(RADIUS, 0, OMEGA); // Ground Truth and Initial/Previous Position
	Vector3d p1(2.0, 0.0, RAD(180.0)), p2(-1.0, 1.7321, RAD(300.0)), p3(-1.0, -1.7321, RAD(60.0)); //x, y, psi

	initialize_sensor(p1, p2, p3);	
	extern VectorXd sensor_data;

	particle_type particle[N_STEP];
	initialize_particle(&particle[0], p1, p2, p3); //initialize particle

	state_type filtered_state[N_STEP];
	filtered_state[0] = expected_state(particle[0]); //initialize expected location

	initialize_visualization(p1, p2, p3);

	/**************************** Loop Part ****************************/	
	for(long t = 1; t < N_STEP; t++){
		p_gt = Simulate_Trajectory(p_gt_old);
		Simulate_Sensor(p_gt, p1, p2, p3);
		add_dependency();
		add_sensor_noise();

		if(t < N_STEP-1){
			particle[t] = particle_motion_update(particle[t-1], p1, p2, p3); //randomly move particles
			particle[t] = particle_sensor_update(particle[t], p1, p2, p3); //update particles' probability
			particle[t] = low_variance_sampler(particle[t]); //sampling new set of particles
			filtered_state[t] = expected_state(particle[t]); //average location
		}
		p_gt_old = p_gt;
		visualization(p_gt, p1, p2, p3, particle[t-1]);
		// printf("Step: %lu\n", t);
		// while(1);
	}

	// matplotlibcpp::plot({1,2,3});
	// matplotlibcpp::show();
	while(1){
		/* Does nothing but smiling at you :) */
		break;
	};
	return 0;
}

/*

	//****************************** Initialization Part ******************************
	map_type map;
	read_beesoft_map(argv[1], &map); //store map data in map variable

	sensor_type sensor;
	read_beesoft_sensor(argv[2], &sensor); //store sensor data in sensor variable

	particle_type particle[sensor.sensor_count];
	particle_initialize(map, &particle[0]); //initialize particle

	state_type filtered_state[sensor.sensor_count];
	filtered_state[0] = expected_state(particle[0]); //initialize expected location

	//**************************** Particle Filtering Part ****************************
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

*/