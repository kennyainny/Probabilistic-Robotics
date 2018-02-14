#include "visualize.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/core/core.hpp> 

using namespace cv;
using namespace std;

void prob_visualize(map_type map, particle_type *particle, state_type *state, long unsigned int step){

   stringstream ss;
   string filename;
   string name = "../results/step_";
   string type = ".jpg";
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

   //cout << prob.size() << endl;  //[423923x0]
   normalize(prob, prob, 0, 255, CV_MINMAX);
   prob.convertTo(prob, CV_8UC1, 1, 0);
   Mat plane[] = {prob, prob, prob};
   merge(plane, 3, prob);
   // printf("Printing\n");
   // cout << prob.size() << endl;
   
   Mat prob2;// = prob.clone();   

   for (int i = 0; i < step; i++)
   {      
      prob2 = prob.clone();  
      for(long unsigned int j = 0; j < particle[0].particle_count; j++){
         MyFilledCircle(prob2, particle[i].state[j].x, particle[i].state[j].y, Scalar( 0, 255, 0), particle[i].prob[j] + 5.0);
         MyFilledCircle(prob2, particle[i].state[j].x, particle[i].state[j].y, Scalar( 0, 0, 255), 3.0);
         // MyFilledCircle(prob, particle[i].state[j].x, particle[i].state[j].y, Scalar( 0, 255*i/step, 255 - 255*i/step ));
         // if(i == step-1){
            // printf("%f %f %f\n", particle[i].state[j].x, particle[i].state[j].y, particle[i].prob[j]);
         // }
      }
      MyFilledCircle(prob2, state[i].x, state[i].y, Scalar( 255, 0, 0), 7.0);
      ss<<name<<(i+1)<<type;
      filename = ss.str();
      ss.str("");
      // printf("%s\n", filename.c_str());
      imwrite(filename, prob2);
   }

   // imwrite("../results/Image.jpg", prob);
   
   // printf("%f %f\n", state[i].x, state[i].y);
   // char a[] =
   // imwrite( "../results/Image.jpg", gray_image );
   // imshow("Current Probability", prob2); 

}

void MyFilledCircle( Mat img, float x, float y, CvScalar color, float r)
{
 int thickness = -1;
 int lineType = 8;
 Point center = Point( x, y);

 circle( img,
         center,
         r,
         color,
         thickness,
         lineType );
}