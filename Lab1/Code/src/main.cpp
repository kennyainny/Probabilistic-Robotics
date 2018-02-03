#include "main.hpp"
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
   printf("Hello, Georgia Tech! \n");

   map_type map;
   read_beesoft_map(argv[1], &map);
   // prob_visualize(map);

   sensor_type sensor;
   read_beesoft_sensor(argv[2], &sensor);

   state_type state;
   sample_motion_model_odometry(sensor.odometry[0], sensor.odometry[1], state);

   waitKey(0);   
   return 0;
}