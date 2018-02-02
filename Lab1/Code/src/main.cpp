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
   prob_visualize(map);

   sensor_type sensor;
   read_beesoft_sensor(argv[2], &sensor);

   waitKey(0);   
   return 0;
}