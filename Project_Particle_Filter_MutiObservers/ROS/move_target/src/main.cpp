#include "main.hpp"

using namespace std;
using namespace Eigen;

int main(int argc, char **argv)
{
	/****************************** Initialization Part ******************************/

	ros::init(argc, argv, "move_target");
	initialize_move_ros();
	ros::Rate rate(1);

	/**************************** Loop Part ****************************/	
	// for(long t = 1; t < N_STEP; t++){
	while(ros::ok()){
	 	moveRobot();
		ros::spinOnce();
	}
	return 0;
}