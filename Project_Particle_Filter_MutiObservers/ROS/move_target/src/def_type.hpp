#ifndef DEF_TYPE_HPP
#define DEF_TYPE_HPP

#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/core/core.hpp> 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <random>
#include <vector>
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float64MultiArray.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Twist.h"

#define N_STEP 2000
#define RADIUS 1.0 // metre
#define SPEED  0.3 // metre/sec
#define OMEGA  SPEED/RADIUS // rad/sec

#define MAX_RANGE 1.75 //metre
#define SENSOR_VIEW 90

#define DEG(rad) (rad*180.0/M_PI)
#define RAD(deg) (deg*M_PI/180.0)

using namespace Eigen;

typedef struct {
  float x, y, theta;
} state_type;

typedef struct {
  state_type *state;
  float *prob;
  unsigned long int particle_count;
} particle_type;

#endif /* DEF_TYPE_H */