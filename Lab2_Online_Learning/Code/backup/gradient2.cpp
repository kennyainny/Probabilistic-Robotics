#include "gradient.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPOCH_NUM 1
#define ALPHA 0.01

using namespace std;

void assign_output(log_type log, long count, int *y, int *y_ind){
  if(log.node_label[count] == VEG){
    *y = 10000;
    *y_ind = 0;
  }else if(log.node_label[count] == WIRE){
    *y = 1000;
    *y_ind = 1;
  }else if(log.node_label[count] == POLE){
    *y = 100;
    *y_ind = 2;
  }else if(log.node_label[count] == GROUND){
    *y = 10;
    *y_ind = 3;
  }else if(log.node_label[count] == FACADE){
    *y = 1;
    *y_ind = 4;
  }
}

void assign_input(log_type log, long count, double *x){
  x[0] = log.point[count].x;
  x[1] = log.point[count].y;
  x[2] = log.point[count].z;
  for(int j = 0; j < 9; j++){
    x[j+3] = log.feature[count].f[j];
  }
}

void assign_weight(int y, double *wx, double *err, double (*w)[O_NUM][W_NUM], double (*w_)[O_NUM][W_NUM], long count, int y_ind){
  double temp_err = 0;
  // for(int i = 0; i < O_NUM; i++){
    temp_err = temp_err + pow((y - wx[y_ind]), 2);
  // }
  temp_err = sqrt(temp_err);
  if(temp_err < err[y_ind]){
    err[y_ind] = temp_err;
    // printf("ERROR %.1f %.4f %.4f %lu\n", y, wx, *err, count);
    // for(int i = 0; i < O_NUM; i++){
      for(int j = 0; j < W_NUM; j++){
        (*w_)[y_ind][j] = (*w)[0][j];
        // printf("WEIGHT %.4f %.4f\n", w[j], w_[j]);
      }
    // }
  }  
}

void multiply_vectors(double *wx, double (*w)[O_NUM][W_NUM], double *x, int y_ind){
  // for(int i = 0; i < O_NUM; i++){
    wx[y_ind] = 0.0;
  // }

  // for (int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++){
      wx[y_ind] = wx[y_ind] + (*w)[0][j]*x[j];
    }    
  // }
}

void update_gradient(log_type log, double (*dl)[O_NUM][W_NUM], double *wx, double *x, int y, long count, int y_ind){
  double DL = 0;
  // for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++)
    {
      (*dl)[y_ind][j] = (wx[y_ind] + log.feature[count].bias - y)*x[j];
      DL = DL + pow((*dl)[y_ind][j], 2.0);
    }
  // }
  DL = sqrt(DL);
  // for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++)
    {
      (*dl)[y_ind][j] = (*dl)[y_ind][j]/DL;
    }
  // }
}

void update_weight(double (*dl)[O_NUM][W_NUM], double (*w)[O_NUM][W_NUM], int y_ind){
  // for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++){ 
      (*w)[0][j] = (*w)[0][j] - ALPHA*(*dl)[y_ind][j];
    }
  // }
}

void assign_result(double *wx, int *y_, int y_ind, double *wx_){
  // int max_ind = 0;
  // for(int i = 1; i < O_NUM; i++){
  //   if(wx[i] > wx[max_ind]){
  //     max_ind = i;
  //   }
  // }
  // for(int i = 1; i < O_NUM; i++){
  //   if(i == max_ind){
  //     *y_ = 1;
  //   }else{
  //     *y_ = 0;
  //   }
  // }
  for(int j = 0; j < O_NUM; j++){
    wx_[j] = (wx[j]-10000/pow(10, j))/(10000/pow(10, j));
  }
}

void Gradient_Descent(log_type train_log, log_type test_log, log_type *gradient_log){
  gradient_log->count = test_log.count;  
  new_hornetsoft_log(gradient_log);
  gradient_log->point = test_log.point;
  gradient_log->node_id = test_log.node_id;

  int y, y_, y_ind;
  double w_[O_NUM][W_NUM], w[O_NUM][W_NUM] = { {1,1,1,1,1,1,1,1,1,1,1,1},
                                               {0,0,0,0,0,0,0,0,0,0,0,0},
                                               {0,0,0,0,0,0,0,0,0,0,0,0},
                                               {0,0,0,0,0,0,0,0,0,0,0,0},
                                               {0,0,0,0,0,0,0,0,0,0,0,0} };
  double wx[O_NUM], wx_[O_NUM], DL, dl[O_NUM][W_NUM], x[W_NUM];
  double err[O_NUM] = {999999999, 999999999, 999999999, 999999999, 999999999};

  /* Training */
  for(int k = 0; k < EPOCH_NUM; k++){
    for(long i = 0; i < train_log.count; i++){
      assign_output(train_log, i, &y, &y_ind);
      assign_input(train_log, i, x);
      multiply_vectors(&wx[y_ind], &w, x, y_ind);
      update_gradient(train_log, &dl, wx, x, y, i, y_ind);
      update_weight(&dl, &w, y_ind);

      if(k == EPOCH_NUM - 1){ //At the last iteration, pick the weight that minimize regret
        multiply_vectors(wx, &w, x, y_ind);
        assign_weight(y, wx, err, &w, &w_, i, y_ind);
        // printf("step: %lu %d %.4f %.4f \n", i, y, wx, w[0]);
        if(i > -1){
          // printf("\n");
          //   for (int j = 0; j < W_NUM; j++){
          //     printf("%.4f ", w[0][j]);
          //   }
          //   printf("\n");
          //   for (int j = 0; j < W_NUM; j++){
          //     printf("%.4f ", w_[y_ind][j]);
          //   }
          // printf("\n");
          //   printf("%.4f ", wx[y_ind]);
          // printf("\n");
          //   printf("%d ", y);
          // printf("\n");
        }
      }
    }
  }

  for(int a = 0; a < 5; a++){
  for(int b = 0; b < 12; b++){
    printf("%.4f ", w_[a][b]);
  }
  printf("\n");
}

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
  for(long i = 0; i < train_log.count; i++){
    assign_output(train_log, i, &y, &y_ind);
    assign_input(train_log, i, x);
    // multiply_vectors(wx, &w_, x, j);

    for(int k = 0; k < O_NUM; k++){      
      wx[k] = 0.0;
      for (int j = 0; j < W_NUM; j++){
      wx[k] = wx[k] + w_[k][j]*x[j];
    }

    }
    for(int j = 0; j < O_NUM; j++){
    wx_[j] = (wx[j]-10000/pow(10, j))/(10000/pow(10, j));
  }
    // assign_result(wx, &y_, y_ind, wx_);
    
    printf("wx: ");
    for (int j = 0; j < O_NUM; j++)
    {
      printf("%.2f ", wx[j]);
    }
    printf("\nwx_: ");
    for (int j = 0; j < O_NUM; j++)
    {
      printf("%.2f ", wx_[j]);
    }
    printf("\ny : ");
      printf("%d", y);
    printf("\n");

    // printf("step: %lu %d %.4f\n", i, y, wx);   
    // if(i == train_log.count - 1){ //At the last iteration, pick the weight that minimize regret
        // printf("step: %lu %d %.4f %.4f \n", i, y, wx, w[0]);
        
          // for(int j = 0; j < weight_size; j++){
          //   printf("%.4f ", w[j]);
          // }
          // printf("\n");
          // for(int j = 0; j < weight_num; j++){
          //   printf("%.4f; ", x[j]);
          // }
          // printf("\n");
          // printf("wx: %lu %d %.4f \n", i, y, wx);
          // printf("\n");
        
      // }

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