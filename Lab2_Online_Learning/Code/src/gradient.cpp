#include "gradient.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPOCH_NUM 500
#define ALPHA 0.01

using namespace std;

void assign_output(log_type log, long count, int *y){
  for(int i = 0; i < O_NUM; i++){
    y[i] = 0;
  }

  if(log.node_label[count] == VEG){
    y[0] = 1;
  }else if(log.node_label[count] == WIRE){
    y[1] = 1;
  }else if(log.node_label[count] == POLE){
    y[2] = 1;
  }else if(log.node_label[count] == GROUND){
    y[3] = 1;
  }else if(log.node_label[count] == FACADE){
    y[4] = 1;
  }
}

void assign_input(log_type log, long count, double *x){
  // x[0] = log.point[count].x;
  // x[1] = log.point[count].y;
  // x[2] = log.point[count].z;
  for(int j = 0; j < 9; j++){
    x[j] = log.feature[count].f[j];
  }
}

void assign_weight(int *y, double *wx, double *err, double (*w)[O_NUM][W_NUM], double (*w_)[O_NUM][W_NUM]){
  double temp_err = 0;
  for(int i = 0; i < O_NUM; i++){
    temp_err = temp_err + pow((y[i] - wx[i]), 2);
  }
  temp_err = sqrt(temp_err);
  if(temp_err < *err){
    *err = temp_err;
    for(int i = 0; i < O_NUM; i++){
      for(int j = 0; j < W_NUM; j++){
        (*w_)[i][j] = (*w)[i][j];
      }
    }
  }  
}

void multiply_vectors(double *wx, double (*w)[O_NUM][W_NUM], double *x){
  for(int i = 0; i < O_NUM; i++){
    wx[i] = 0.0;
  }

  for (int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++){
      wx[i] = wx[i] + (*w)[i][j]*x[j];
    }    
  }
}

void update_gradient(log_type log, double (*dl)[O_NUM][W_NUM], double *wx, double *x, int *y){
  for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++)
    {
      (*dl)[i][j] = (wx[i]- y[i])*x[j]/log.count;
    }
  }
}

void update_weight(double (*dl)[O_NUM][W_NUM], double (*w)[O_NUM][W_NUM]){
  for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++){ 
      (*w)[i][j] = (*w)[i][j] - ALPHA*(*dl)[i][j];
    }
  }
}

void assign_result(double *wx, int *y_){
  int max_ind = 0;
  for(int i = 1; i < O_NUM; i++){
    if(wx[i] > wx[max_ind]){
      max_ind = i;
    }
  }
  for(int i = 1; i < O_NUM; i++){
    if(i == max_ind){
      y_[i] = 1;
    }else{
      y_[i] = 0;
    }
  }
}

void Gradient_Descent(log_type train_log, log_type test_log, log_type *gradient_log){
  int y[O_NUM] = {0}, y_[O_NUM] = {0};
  double w[O_NUM][W_NUM] = { {0,0,0,0,0,0,0,0,0},
                             {0,0,0,0,0,0,0,0,0},
                             {0,0,0,0,0,0,0,0,0},
                             {0,0,0,0,0,0,0,0,0},
                             {0,0,0,0,0,0,0,0,0} };
  double wx[O_NUM], DL, dl[O_NUM][W_NUM], x[W_NUM];
  double err = 9999999, w_[O_NUM][W_NUM];

  /* Training */
  for(int k = 0; k < EPOCH_NUM; k++){
    for(long i = 0; i < train_log.count; i++){
      assign_output(train_log, i, y);
      assign_input(train_log, i, x);
      multiply_vectors(wx, &w, x);
      update_gradient(train_log, &dl, wx, x, y);
      update_weight(&dl, &w);

      if(k == EPOCH_NUM - 1){ //At the last iteration, pick the weight that minimize regret
        // multiply_vectors(wx, &w, x);
        // assign_weight(y, wx, &err, &w, &w_, i);
        // if(i > -1){
        //   printf("\n");
        //   for(int i = 0; i < O_NUM; i++){
        //     for (int j = 0; j < W_NUM; j++){
        //       printf("%.4f ", w[i][j]);
        //     }
        //     printf("\n");
        //   }
        //   for(int j = 0; j < O_NUM; j++){
        //     printf("%.4f ", wx[j]);
        //   }
        //   printf("\n");
        //   for(int j = 0; j < O_NUM; j++){
        //     printf("%d ", y[j]);
        //   }
        // }
      }
    }
  }

  /* Testing */
  gradient_log->count = test_log.count;  
  new_hornetsoft_log(gradient_log);
  
  for(long i = 0; i < test_log.count; i++){
    gradient_log->point[i] = test_log.point[i];
    gradient_log->node_id[i] = test_log.node_id[i];

    assign_output(test_log, i, y);
    assign_input(test_log, i, x);
    // multiply_vectors(wx, &w_, x);
    // assign_result(wx, y_);

    multiply_vectors(wx, &w, x);
        if(i > -1){
          printf("\n");
          for(int i = 0; i < O_NUM; i++){
            for (int j = 0; j < W_NUM; j++){
              printf("%.4f ", w[i][j]);
            }
            printf("\n");
          }
          for(int j = 0; j < O_NUM; j++){
            printf("%.4f ", wx[j]);
          }
          printf("\n");
          for(int j = 0; j < O_NUM; j++){
            printf("%d ", y[j]);
          }
        }
    if(wx[0] > 0.2){
      gradient_log->node_label[i] = VEG;
    }else if(wx[1] > 0.025){
      gradient_log->node_label[i] = WIRE;
    }else if(wx[2] > 0.025){
      gradient_log->node_label[i] = POLE;
    }else if(wx[4] > 0.2){
      gradient_log->node_label[i] = FACADE;
    }else{
      gradient_log->node_label[i] = GROUND;
    }    
  }
}