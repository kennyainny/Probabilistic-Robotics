#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include "def_type.hpp"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"

using namespace Eigen;

// void moveRobot();
// void stopRobot();
void odomCallback(const nav_msgs::Odometry::ConstPtr& odom);
void initialize_odom_ros();
Vector3d Simulate_Trajectory(Vector3d v0);


#endif /* TRAJECTORY_HPP */