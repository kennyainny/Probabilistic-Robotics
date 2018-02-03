#include "visualize.hpp"
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void prob_visualize(map_type map){

	Mat prob = Mat::zeros(map.size_x,map.size_y, CV_32F);

	for(int i = 0; i <= map.size_x-1; i++){
    	float *ptr = prob.ptr<float>(i);

   	for(int j = 0; j <= map.size_y-1; j++){
   		// printf("%.2f ",map.prob[i][j]);

         if(map.prob[i][j] > 0){
            ptr[j] = map.prob[i][j];
         }

   	}
   	//printf(" \n");
   }
   
   //cout << prob.size() << endl;
   normalize(prob, prob, 0, 255, CV_MINMAX);
   prob.convertTo(prob, CV_8UC1, 1, 0);
   
   imshow("Current Probability", prob);

}