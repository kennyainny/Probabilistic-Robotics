#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include "def_type.hpp"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"

using namespace Eigen;

void moveRobot();
void stopRobot();
void initialize_move_ros();


#endif /* TRAJECTORY_HPP */