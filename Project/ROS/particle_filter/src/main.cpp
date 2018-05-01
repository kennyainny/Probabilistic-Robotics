#include "main.hpp"

using namespace std;
using namespace Eigen;

int main(int argc, char **argv)
{
	/****************************** Initialization Part ******************************/
	
	Vector3d p_gt, p_gt_old(RADIUS, 0, OMEGA); // Ground Truth and Initial/Previous Position
	Vector3d p1(2.0, 0.0, RAD(180.0)), p2(-1.0, 1.7321, RAD(300.0)), p3(-1.0, -1.7321, RAD(60.0)); //x, y, psi
	
	extern VectorXd sensor_data;
	extern VectorXd sensor_data_gazebo;
	extern float odom_x, odom_y;
	// initialize_sensor(p1, p2, p3);	

	ros::init(argc, argv, "arraySubscriber");	
	initialize_sensor_ros();
	initialize_odom_ros();
	ros::Rate rate(1);

	// particle_type particle[N_STEP];
	particle_type particle[2];
	initialize_particle(&particle[0], p1, p2, p3); //initialize particle

	// state_type filtered_state[N_STEP];
	state_type filtered_state[2];
	filtered_state[0] = expected_state(particle[0]); //initialize expected location

	initialize_visualization(p1, p2, p3);

	/**************************** Loop Part ****************************/	
	// for(long t = 1; t < N_STEP; t++){
	// while(1){
	while(ros::ok()){
		p_gt(0) = odom_x;
		p_gt(1) = odom_y;
	 	// moveRobot(); //publish robot's vel
		// p_gt = Simulate_Trajectory(p_gt_old);
		// Simulate_Sensor(p_gt, p1, p2, p3);
		// add_dependency();
		// add_sensor_noise();

		particle[1] = particle_motion_update(particle[0], p1, p2, p3); //randomly move particles
		particle[1] = particle_sensor_update(particle[1], p1, p2, p3); //update particles' probability
		particle[1] = low_variance_sampler(particle[1]); //sampling new set of particles
		filtered_state[1] = expected_state(particle[1]); //average location
		visualization(p_gt, p1, p2, p3, particle[0], filtered_state[0]);

		particle[0] = particle[1];
		filtered_state[0] = filtered_state[1];

		// if(t < N_STEP-1){
			// particle[t] = particle_motion_update(particle[t-1], p1, p2, p3); //randomly move particles
			// particle[t] = particle_sensor_update(particle[t], p1, p2, p3); //update particles' probability
			// particle[t] = low_variance_sampler(particle[t]); //sampling new set of particles
			// filtered_state[t] = expected_state(particle[t]); //average location
		// }
		// p_gt_old = p_gt;

		

		// printf("aaa\n");

		// for(int i = 0; i < (SENSOR_VIEW+1)*3; i++){
		// 	printf("%.3f, %.2f, %.2f", sensor_data[i], odom_x, odom_y);
		// }		

		// printf("%lu\n", t);

		ros::spinOnce();
		// rate.sleep();

	}
	// ros::spin();
	// while(1){
		/* Does nothing but smiling at you :) */
		// break;
	// };
	return 0;
}