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
   int thickness = -1;
   Point center = Point( y, x);
   circle(img, center, r, color, thickness);
}

void MyFilledCircle_Transparent( Mat img, float x, float y, CvScalar color, float r)
{
   Point center = Point( y, x);
   circle(img, center, r, color);
}

void particle_visualize(particle_type particle, state_type state, laser_type laser, map_type map, int step){

   Mat prob = Mat::zeros(map.size_x, map.size_y, CV_32F);
   for(int i = 0; i < map.size_x; i++){
      float *ptr = prob.ptr<float>(i);

      for(int j = 0; j < map.size_y; j++){

         if(map.prob[i][j] >= 0){
            ptr[j] = map.prob[i][j];
         }
         if(map.prob[i][j] == -1){
            ptr[j] = 0;
         }
      }
   }

   normalize(prob, prob, 0, 255, CV_MINMAX);
   prob.convertTo(prob, CV_8UC1, 1, 0);
   Mat plane[] = {prob, prob, prob};
   merge(plane, 3, prob);

   Mat prob2;

   prob2 = prob.clone();  
   for(unsigned long int i = 0; i < particle.particle_count; i++){
      MyFilledCircle_Transparent(prob2, particle.state[i].x, particle.state[i].y, Scalar( 0, 255, 0), particle.prob[i]*80000.0);
      MyFilledCircle(prob2, particle.state[i].x, particle.state[i].y, Scalar( 0, 0, 255), 7.0);
   }

   state.x = state.x + (25/map.resolution)*cos(state.theta);
   state.y = state.y + (25/map.resolution)*sin(state.theta);

   for (int i = 0; i < 180; i++){
      float angle = RAD((float)i) + state.theta;
      for (int j = 0; j < (int)(laser.r[i]/map.resolution); j++){
         int x_ray_casting = state.x + (j * cos(angle - M_PI / 2));
         int y_ray_casting = state.y + (j * sin(angle - M_PI / 2));
         if (x_ray_casting > 0 && x_ray_casting < map.size_x && 
            y_ray_casting > 0 && y_ray_casting < map.size_y){
            MyFilledCircle(prob2, x_ray_casting, y_ray_casting, Scalar(0, 255, 255), 1);
         }
      }
   }

   MyFilledCircle(prob2, state.x, state.y, Scalar( 255, 0, 0), 5.0);

   imshow("Current Probability", prob2);
   save_image(prob2, step);
   waitKey( 10 );
}

void save_image(Mat img, int step){
   stringstream ss;
   string filename;
   string name = "../results/step_";
   string type = ".jpg";

   ss<<name<<(step)<<type;
   filename = ss.str();
   ss.str("");
   imwrite(filename, img);
}
