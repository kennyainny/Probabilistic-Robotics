#include "main.hpp"

using namespace std;
using namespace Eigen;

int main(int argc, char *argv[])
{
	/****************************** Initialization Part ******************************/
	Vector3d v_gt, v_gt_old(RADIUS, 0, OMEGA); // Ground Truth and Initial/Previous Position
	Vector3d v1(2.0, 0.0, RAD(180.0)), v2(-1.0, 1.7321, RAD(-60.0)), v3(-1.0, -1.7321, RAD(60.0)); //x, y, psi
	initialize_visualization(v1, v2, v3);

	/**************************** Loop Part ****************************/	
	for(long t = 0; t < N_STEP; t++){
		v_gt = Simulate_Trajectory(v_gt_old);
		v_gt_old = v_gt;

		visualization(v_gt);
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