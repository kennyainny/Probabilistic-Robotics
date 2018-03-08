#include "gradient.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Y_VEG    0
#define Y_WIRE   1
#define Y_POLE   0
#define Y_GROUND 0
#define Y_FACADE 0

#define EPOCH_NUM 500

#define ALPHA 0.01

double assign_output(log_type log, long count){
  int y;

  if(log.node_label[count] == VEG){
      y = Y_VEG;
    }else if(log.node_label[count] == WIRE){
      y = Y_WIRE;
    }else if(log.node_label[count] == POLE){
      y = Y_POLE;
    }else if(log.node_label[count] == GROUND){
      y = Y_GROUND;
    }else if(log.node_label[count] == FACADE){
      y = Y_FACADE;
    }

  return (double)y;
}

void assign_input(log_type log, long count, double *x){

  // x[0] = log.point[count].x*0.0001;
  // x[1] = log.point[count].y*0.0001;
  // x[2] = log.point[count].z*0.0001;
  for(int j = 0; j < 9; j++){
    x[j] = log.feature[count].f[j];
  }
  // x[7] = x[7]*0.1;
}

void assign_weight(double y, double wx, double *err, double *w, double *w_, int weight_size, long count){
  if(pow((y - wx),2) < *err){
    *err = pow((y - wx),2);
    // printf("ERROR %.1f %.4f %.4f %lu\n", y, wx, *err, count);
    for(int j = 0; j < weight_size; j++){
      w_[j] = w[j];
      // printf("WEIGHT %.4f %.4f\n", w[j], w_[j]);
    }
  }
}

double multiply_vectors(double *w, double *x, int weight_size){
  double wx = 0;
  for (int j = 0; j < weight_size; j++)
  {
    wx = wx + w[j]*x[j];
  }
  return wx;
}

void update_gradient(log_type log, double *dl, double *w, double *x, double y, int weight_size, long count){
  double DL = 0, wx = 0;
  wx = multiply_vectors(w, x, weight_size);
  for (int j = 0; j < weight_size; j++)
  {
    dl[j] = (wx - y)*x[j]/89822;
    // DL = DL + pow(dl[j], 2.0);
  }
  // DL = 1;//sqrt(DL);
  for (int j = 0; j < weight_size; j++)
  { 
    dl[j] = dl[j];///DL;
  }
}

void update_weight(double *dl, double *w, int weight_size){
  for (int j = 0; j < weight_size; j++){ 
    w[j] = w[j] - ALPHA*dl[j];
  }
}

void Gradient_Descent(log_type train_log, log_type test_log, log_type *gradient_log){
  int y, weight_size = 9;
  double w[weight_size] = {0};
  double wx, l, DL, dl[weight_size], x[weight_size];
  double err_veg = 9999999, err_wire = 9999999, err_pole = 9999999, err_ground = 9999999, err_facade = 9999999, err = 9999999;
  double w_veg[weight_size], w_wire[weight_size], w_pole[weight_size], w_ground[weight_size], w_facade[weight_size], w_[weight_size];

  /* Training */
  for(int k = 0; k < EPOCH_NUM; k++){
    for(long i = 0; i < train_log.count; i++){
      y = assign_output(train_log, i);
      assign_input(train_log, i, x);
      update_gradient(train_log, dl, w, x, y, weight_size, i);
      update_weight(dl, w, weight_size);      

      if(k == EPOCH_NUM - 1){ //At the last iteration, pick the weight that minimize regret
        wx = multiply_vectors(w, x, weight_size);
        if(y==1){
          assign_weight(y, wx, &err, w, w_, weight_size, i);
        }
        // if(y == Y_VEG){
        //   assign_weight(y, wx, &err_veg, w, w_veg, weight_size, i);
        // }else if(y == Y_WIRE){
        //   assign_weight(y, wx, &err_wire, w, w_wire, weight_size, i);
        // }else if(y == Y_POLE){
        //   assign_weight(y, wx, &err_pole, w, w_pole, weight_size, i);
        // }else if(y == Y_GROUND){
        //   assign_weight(y, wx, &err_ground, w, w_ground, weight_size, i);
        // }else if(y == Y_FACADE){
        //   assign_weight(y, wx, &err_facade, w, w_facade, weight_size, i);
        // }
        // printf("step: %lu %d %.4f %.4f \n", i, y, wx, w[0]);
        if(i > -1){
          // for(int j = 0; j < weight_size; j++){
          //   printf("%.4f ", w[j]);
          // }
          // printf("\n");
          // for(int j = 0; j < weight_size; j++){
          //   printf("%.4f; ", x[j]);
          // }
          // printf("\n");
          // printf("i y wx: %lu %d %.4f \n",i , y, wx);
          // printf("\n");
        }
      }
    }
  }

  for(int j = 0; j < weight_size; j++){
    // printf("%d %.4f %.4f %.4f %.4f %.4f\n", j, w_veg[j], w_wire[j], w_pole[j], w_ground[j], w_facade[j]);
    // printf("%.4f ", w_[j]);
  }
  // printf("\n");

  // printf("\n");
  // for(long i = 0; i < train_log.count; i++){
  //   assign_input(train_log, i, x);
  //   y = assign_output(train_log, i);
  //   wx = multiply_vectors(w, x, weight_size);
  //   // printf("step: %lu %d %.4f %.4f \n", i, y, wx, w[0]);
  //   if(i > train_log.count - 2){
  //     for(int j = 0; j < weight_size; j++){
  //       printf("x and w: %.4f %.4f \n", x[j], w_[j]);
  //     }
  //     printf("wx: %d %.4f \n", y, wx);
  //   }
  // }  
  // while(1);

  /* Testing */
  gradient_log->count = train_log.count;
  new_hornetsoft_log(gradient_log); 
  for(long i = 0; i < train_log.count; i++){    
    
    gradient_log->point[i] = train_log.point[i];
    gradient_log->node_id[i] = train_log.node_id[i];

    y = assign_output(train_log, i);
    assign_input(train_log, i, x);

    wx = multiply_vectors(w_, x, weight_size);

    if(wx > 0.2){
      gradient_log->node_label[i] = WIRE;
    }else{
      gradient_log->node_label[i] = GROUND;
    }
        // assign_weight(1, wx, &err, w, w_, weight_size, i);
        // if(y == Y_VEG){
        //   assign_weight(y, wx, &err_veg, w, w_veg, weight_size, i);
        // }else if(y == Y_WIRE){
        //   assign_weight(y, wx, &err_wire, w, w_wire, weight_size, i);
        // }else if(y == Y_POLE){
        //   assign_weight(y, wx, &err_pole, w, w_pole, weight_size, i);
        // }else if(y == Y_GROUND){
        //   assign_weight(y, wx, &err_ground, w, w_ground, weight_size, i);
        // }else if(y == Y_FACADE){
        //   assign_weight(y, wx, &err_facade, w, w_facade, weight_size, i);
        // }
        // printf("step: %lu %d %.4f %.4f \n", i, y, wx, w[0]);
        // // if(i == train_log.count-1){
        //   for(int j = 0; j < weight_size; j++){
        //     printf("%.4f ", w[j]);
        //   }
        //   printf("\n");
        //   for(int j = 0; j < weight_size; j++){
        //     printf("%.4f; ", x[j]);
        //   }
        //   printf("\n");
        //   printf("i y wx: %lu %d %.4f \n",i , y, wx);
        //   printf("\n");
        // }
      

    // wx = 0;
    // for (int j = 0; j < weight_size; j++){
    //   wx = wx + w_veg[j]*x[j];
    // }
    // printf("%.2f %.2f\n", wx, (wx-Y_VEG));
    // wx = 0;
    // for (int j = 0; j < weight_size; j++){
    //   wx = wx + w_wire[j]*x[j];
    // }
    // printf("%.2f %.2f\n", wx, (wx-Y_WIRE));
    // wx = 0;
    // for (int j = 0; j < weight_size; j++){
    //   wx = wx + w_pole[j]*x[j];
    // }
    // printf("%.2f %.2f\n", wx, (wx-Y_POLE)); 
    // wx = 0;
    // for (int j = 0; j < weight_size; j++){
    //   wx = wx + w_ground[j]*x[j];
    // }
    // printf("%.2f %.2f\n", wx, (wx-Y_GROUND)); 
    // wx = 0;
    // for (int j = 0; j < weight_size; j++){
    //   wx = wx + w_facade[j]*x[j];
    // }
    // printf("%.2f %.2f\n", wx, (wx-Y_FACADE));
    

    // if(wx < Y_FACADE){
    //   gradient_log->node_label[i] = FACADE;
    // }else if(wx > Y_FACADE && wx < Y_GROUND){
    //   if(abs(wx - Y_FACADE) < abs(wx - Y_GROUND)){
    //     gradient_log->node_label[i] = FACADE;
    //   }else{
    //     gradient_log->node_label[i] = GROUND;
    //   }      
    // }else if(wx > Y_GROUND && wx < Y_POLE){
    //   if(abs(wx - Y_GROUND) < abs(wx - Y_POLE)){
    //     gradient_log->node_label[i] = GROUND;
    //   }else{
    //     gradient_log->node_label[i] = POLE;
    //   }  
    // }else if(wx > Y_POLE && wx < Y_WIRE){
    //   if(abs(wx - Y_POLE) < abs(wx - Y_WIRE)){
    //     gradient_log->node_label[i] = POLE;
    //   }else{
    //     gradient_log->node_label[i] = WIRE;
    //   }  
    // }else if(wx > Y_WIRE && wx < Y_VEG){
    //   if(abs(wx - Y_WIRE) < abs(wx - Y_VEG)){
    //     gradient_log->node_label[i] = WIRE;
    //   }else{
    //     gradient_log->node_label[i] = VEG;
    //   }  
    // }else if(wx > Y_VEG){
    //   gradient_log->node_label[i] = VEG;
    // }

    // printf("step: %d %d %.4f %d\n", i, y, wx, gradient_log->node_label[i]);    
  }
}