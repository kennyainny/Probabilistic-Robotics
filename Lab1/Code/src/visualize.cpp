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

void MyFilledCircle( Mat img, float x, float y, CvScalar color, float r)
{
 // int thickness = -1;
 // int lineType = 8;
 // Point center = Point( y, x);

 // circle( img, center, r, color, thickness, lineType );
}

void particle_visualize(particle_type particle, state_type filtered_state, laser_type laser, map_type map){
   printf("bbb\n");
   // Mat prob = Mat::zeros(map.size_x, map.size_y, CV_32F);
   // for(int i = 0; i < map.size_x; i++){
   //    float *ptr = prob.ptr<float>(i);

   //    for(int j = 0; j < map.size_y; j++){

   //       if(map.prob[j][i] >= 0){
   //          ptr[j] = map.prob[j][i];
   //       }
   //       if(map.prob[j][i] == -1){
   //          ptr[j] = 0;
   //       }
   //    }
   // }

   // // prob = flip(prob, 1);
   // normalize(prob, prob, 0, 255, CV_MINMAX);
   // prob.convertTo(prob, CV_8UC1, 1, 0);
   // Mat plane[] = {prob, prob, prob};
   // merge(plane, 3, prob);

   // imshow("Current Probability", prob); 
   // waitKey( 10 );
}

void prob_visualize(map_type map, particle_type *particle, state_type *state, unsigned long int step){

 //   stringstream ss;
 //   string filename;
 //   string name = "../results/step_";
 //   string type = ".jpg";
	// Mat prob = Mat::zeros(map.size_x,map.size_y, CV_32F);

	// for(int i = 0; i < map.size_x; i++){
 //    	float *ptr = prob.ptr<float>(i);

 //   	for(int j = 0; j < map.size_y; j++){
 //   		// printf("%.2f ",map.prob[i][j]);

 //         if(map.prob[i][j] >= 0){
 //            ptr[j] = map.prob[i][j];
 //         }
 //         if(map.prob[i][j] == -1){
 //            ptr[j] = 0;
 //         }
 //         // if(map.prob[j][i] > 0 && map.prob[j][i] < 1){
 //         //    ptr[j] = 0.5;
 //         // }

 //   	}
 //   	//printf(" \n");
 //   }

 //   //cout << prob.size() << endl;  //[423923x0]
 //   normalize(prob, prob, 0, 255, CV_MINMAX);
 //   prob.convertTo(prob, CV_8UC1, 1, 0);
 //   Mat plane[] = {prob, prob, prob};
 //   merge(plane, 3, prob);
 //   // printf("Printing\n");
 //   // cout << prob.size() << endl;
   
 //   Mat prob2;// = prob.clone();   

 //   // step = 1;
 //   for (int i = 0; i < step; i++)
 //   {      
 //      prob2 = prob.clone();  
 //      for(unsigned long int j = 0; j < particle[0].particle_count; j++){
 //         // printf("%f %f %f\n", particle[i].state[j].x, particle[i].state[j].y, particle[i].prob[j]);
 //         MyFilledCircle(prob2, particle[i].state[j].x, particle[i].state[j].y, Scalar( 0, 255, 0), particle[i].prob[j] + 3.0);
 //         // MyFilledCircle(prob2, particle[i].state[j].x, particle[i].state[j].y, Scalar( 0, 0, 255), 3.0);
 //         // MyFilledCircle(prob, particle[i].state[j].x, particle[i].state[j].y, Scalar( 0, 255*i/step, 255 - 255*i/step ));
 //         // if(i == step-1){
 //            // printf("%f %f %f\n", particle[i].state[j].x, particle[i].state[j].y, particle[i].prob[j]);
 //         // }
 //      }
 //      MyFilledCircle(prob2, state[i].x, state[i].y, Scalar( 255, 0, 0), 7.0);
 //      ss<<name<<(i+1)<<type;
 //      filename = ss.str();
 //      ss.str("");
 //      // printf("%s\n", filename.c_str());
 //      imwrite(filename, prob2);
 //   }
}

