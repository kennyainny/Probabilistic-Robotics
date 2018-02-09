#include "visualize.hpp"
#include <iostream>
#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/core/core.hpp> 

using namespace cv;
using namespace std;

void prob_visualize(map_type map, particle_type *particle, state_type *state, long unsigned int step){

	Mat prob = Mat::zeros(map.size_x,map.size_y, CV_32F);

	for(int i = 0; i <= map.size_x-1; i++){
    	float *ptr = prob.ptr<float>(i);

   	for(int j = 0; j <= map.size_y-1; j++){
   		// printf("%.2f ",map.prob[i][j]);

         if(map.prob[j][i] >= 0){
            ptr[j] = map.prob[j][i];
         }
         if(map.prob[j][i] == -1){
            ptr[j] = 1;
         }
         // if(map.prob[j][i] > 0 && map.prob[j][i] < 1){
         //    ptr[j] = 0.5;
         // }

   	}
   	//printf(" \n");
   }
   
   // Mat aaa = Mat::zeros(100, 100, CV_8UC1);
   //cout << prob.size() << endl;  //[423923x0]
   normalize(prob, prob, 0, 255, CV_MINMAX);
   prob.convertTo(prob, CV_8UC1, 1, 0);
   printf("Printing\n");
   cout << prob.size() << endl;
   
   Mat plane[] = {prob, prob, prob};
   merge(plane, 3, prob);

   // for(long unsigned int i = 0; i < step; i++){
   //    MyFilledCircle(prob, state[i].x, state[i].y);
   // }

   for (int i = 0; i < step; i++)
   {
      for(long unsigned int j = 0; j < particle[0].particle_count; j++){
         MyFilledCircle(prob, particle[i].state[j].x, particle[i].state[j].y, Scalar( 0, 255*i/step, 255 - 255*i/step ));
      }
   }
   
   
   imshow("Current Probability", prob); //Error occurs here Asertion failed 

}

void MyFilledCircle( Mat img, float x, float y, CvScalar color)
{
 int thickness = -1;
 int lineType = 8;
 Point center = Point( x, y);

 circle( img,
         center,
         1.0,
         color,
         thickness,
         lineType );
}