#include "main.hpp"

using namespace std;
using namespace Eigen;

int main(int argc, char *argv[])
{
	/****************************** Initialization Part ******************************/
	Vector3d p_gt, p_gt_old(RADIUS, 0, OMEGA); // Ground Truth and Initial/Previous Position
	Vector3d p1(2.0, 0.0, RAD(180.0)), p2(-1.0, 1.7321, RAD(300.0)), p3(-1.0, -1.7321, RAD(60.0)); //x, y, psi
	initialize_sensor(p1, p2, p3);
	initialize_visualization(p1, p2, p3);
	extern VectorXd sensor_data;

	/**************************** Loop Part ****************************/	
	for(long t = 0; t < N_STEP; t++){
		p_gt = Simulate_Trajectory(p_gt_old);
		Simulate_Sensor(p_gt, p1, p2, p3);
		add_sensor_noise();
		p_gt_old = p_gt;

		visualization(p_gt, p1, p2, p3);
		// printf("Step: %lu\n", t);
	}

	// matplotlibcpp::plot({1,2,3});
	// matplotlibcpp::show();
	while(1){
		/* Does nothing but smiling at you :) */
		break;
	};
	return 0;
}