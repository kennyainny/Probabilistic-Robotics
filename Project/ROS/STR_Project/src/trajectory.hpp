#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include "def_type.hpp"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"

using namespace Eigen;

Vector3d Simulate_Trajectory(Vector3d v0);
int odom_main(int argcc, char **argvv);
void odomCallback(const nav_msgs::Odometry::ConstPtr& odom);
int move_robot(int arccc,char **argvvv);


#endif /* TRAJECTORY_HPP */