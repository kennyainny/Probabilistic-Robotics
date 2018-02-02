#include "main.h"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;

int main(int argc, char *argv[]) //char **argv[]
{
   printf("Hello, World!");

   map_type map;
   read_beesoft_map(argv[1], &map);

   // int i = 1, j = 1;

   for(int i = 1; i <= map.size_x; i++){
   	for(int j = 1; j <= map.size_y; j++){
   		printf("%.2f ",map.prob[i][j]);
   	}
   	printf(" \n");
   }
   imshow( map.prob,image);
   imshow("Gray image", image);
   waitKey(0)


   return 0;
}