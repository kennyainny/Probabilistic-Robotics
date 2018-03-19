#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/core/core.hpp> 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "matplotlibcpp.h"

#ifndef DEF_TYPE_HPP
#define DEF_TYPE_HPP

#define N_STEP 1000
#define RADIUS 1.0 // metre
#define SPEED  10.0 // metre/sec
#define OMEGA  SPEED/RADIUS // rad/sec

#define MAX_RANGE 1.8 //metre

#define DEG(rad)  (rad*180.0/M_PI)
#define RAD(deg)  (deg*M_PI/180.0)

#endif /* DEF_TYPE_H */