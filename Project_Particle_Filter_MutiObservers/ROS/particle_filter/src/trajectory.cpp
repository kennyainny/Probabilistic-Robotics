#include "trajectory.hpp"

#define TIME_STEP 0.03 // sec/step

using namespace std;
using namespace Eigen;

// geometry_msgs::Pose2D current_pose;
// ros::Publisher movement_pub;
ros::Subscriber odom_sub;

float odom_x;
float odom_y;

// float linearSpeed = -0.5; //m/s
// float angularSpeed = 0.5;
// float t_increm = 0.1;

// void moveRobot(){
// 	geometry_msgs::Twist move; //velocity controls
	
// 	move.linear.x = linearSpeed; //speed value m/s 
// 	move.linear.y = 0;
// 	move.angular.z = angularSpeed;
	
// 	// printf("[vel, omega] -> [%f, %f]\n", linearSpeed, angularSpeed);

// 	movement_pub.publish(move);
	
// 	// ros::Duration(0.05).sleep();   //sampling time
// }

// void stopRobot(){
// 	geometry_msgs::Twist move; //velocity controls
	
// 	move.linear.x = 0.0; //linearSpeed; //speed value m/s 
// 	move.linear.y = 0.0;
// 	move.angular.z = 0.0; //angularSpeed;
	
// 	// printf("[vel, omega] -> [%f, %f]\n",linearSpeed,angularSpeed);

// 	movement_pub.publish(move);
	
// 	// ros::Duration(5).sleep();   //sampling time
// }

void odomCallback(const nav_msgs::Odometry::ConstPtr& odom){

	// linear position
    // current_pose.x = odom -> pose.pose.position.x;
    // current_pose.y = odom -> pose.pose.position.y;
  	
	odom_x = odom -> pose.pose.position.x;
  	odom_y = odom -> pose.pose.position.y;
}

void initialize_odom_ros(){
	ros::NodeHandle no;//, nm;
  	odom_sub = no.subscribe("target/odom", 1, odomCallback); //change to target_odom
	// movement_pub = nm.advertise<geometry_msgs::Twist>("target/cmd_vel", 1);
}

Vector3d Simulate_Trajectory(Vector3d p_gt_old){
	Vector3d p(0, 0, OMEGA); //x , y, Omega
	Matrix3d R;
	R = AngleAxisd(OMEGA*TIME_STEP, Vector3d::UnitZ());
	// R = AngleAxisd(RAD(90), Vector3d::UnitZ());
	p = R*p_gt_old;

	return p;
}
