#include "trajectory.hpp"

#define TIME_STEP 0.01 // sec/step

using namespace std;
using namespace Eigen;

// geometry_msgs::Pose2D current_pose;
ros::Publisher movement_pub;
ros::Subscriber odom_sub;

float odom_x;
float odom_y;

float linearSpeed = -0.1; //m/s
float angularSpeed = 0.1;
float t_increm = 0.1;

void moveRobot(){
	geometry_msgs::Twist move; //velocity controls
	
	move.linear.x = linearSpeed; //speed value m/s 
	move.linear.y = 0;
	move.angular.z = angularSpeed;
	
	// printf("[vel, omega] -> [%f, %f]\n", linearSpeed, angularSpeed);

	movement_pub.publish(move);
	
	// ros::Duration(0.05).sleep();   //sampling time
}

void stopRobot(){
	geometry_msgs::Twist move; //velocity controls
	
	move.linear.x = 0.0; //linearSpeed; //speed value m/s 
	move.linear.y = 0.0;
	move.angular.z = 0.0; //angularSpeed;
	
	// printf("[vel, omega] -> [%f, %f]\n",linearSpeed,angularSpeed);

	movement_pub.publish(move);
	
	// ros::Duration(5).sleep();   //sampling time
}

void initialize_move_ros(){
	ros::NodeHandle nm;
	movement_pub = nm.advertise<geometry_msgs::Twist>("target/cmd_vel", 1);
}
