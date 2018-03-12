#include "gradient.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPOCH_NUM 500

using namespace std;

void assign_output(log_type log, long count, int *y, int *type){
  for(int i = 0; i < O_NUM; i++){
    y[i] = 0;
  }

  if(log.node_label[count] == VEG){
    y[0] = 1;
    *type = 0;
  }else if(log.node_label[count] == WIRE){
    y[1] = 1;
    *type = 1;
  }else if(log.node_label[count] == POLE){
    y[2] = 1;
    *type = 2;
  }else if(log.node_label[count] == GROUND){
    y[3] = 1;
    *type = 3;
  }else if(log.node_label[count] == FACADE){
    y[4] = 1;
    *type = 4;
  }
}

void assign_input(log_type log, long count, double *x){
  for(int j = 0; j < 9; j++){
    x[j] = log.feature[count].f[j];
  }
  x[9] = log.feature[count].bias;
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

void update_gradient(double (*dl)[O_NUM][W_NUM], double *wx, double *x, int *y){
  double DL = 0;
  for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++){
      (*dl)[i][j] = (wx[i] - y[i])*x[j];
      DL = DL + pow((*dl)[i][j], 2);
    }
  }
  DL = sqrt(DL);
  for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++){
      (*dl)[i][j] = (*dl)[i][j]/DL;
    }
  }
}

void update_weight(double (*dl)[O_NUM][W_NUM], double (*w)[O_NUM][W_NUM], double alpha){
  for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++){ 
      (*w)[i][j] = (*w)[i][j] - alpha*(*dl)[i][j];
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

void store_weight(double (*w)[O_NUM][W_NUM], double (*w_store)[O_NUM][W_NUM][O_NUM], int type){
  for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++){
      (*w_store)[i][j][type] = (*w)[i][j];
    }
  }
}

double loss_calc(double (*w)[O_NUM][W_NUM], double *x, int *y){
  double wx[O_NUM], sum = 0;
  multiply_vectors(wx, w, x);
  for(int i = 0; i < O_NUM; i++){
    sum = sum + pow((y[i] - wx[i]), 2);
  }
  return sum*0.5;
}

void assign_weight(int *y, double *x, double (*w)[O_NUM][W_NUM], double (*w_)[O_NUM][W_NUM], double (*w_store)[O_NUM][W_NUM][O_NUM], double *sum_loss, int type){
  double loss, loss_, loss_temp, w_temp[O_NUM][W_NUM];
  loss  = loss_calc(w, x, y);

  for(int i = 0; i < O_NUM; i++){
    for (int j = 0; j < W_NUM; j++){
      w_temp[i][j] = (*w_store)[i][j][type];
    }
  }
  
  loss_temp = loss_calc(&w_temp, x, y);
    
  if(loss < loss_temp){
    loss_ = loss;
    store_weight(w, w_store, type);
    for(int i = 0; i < O_NUM; i++){
      for (int j = 0; j < W_NUM; j++){
        (*w_)[i][j] = (*w)[i][j];
      }
    }
  }else{
    loss_ = loss_temp;
    for(int i = 0; i < O_NUM; i++){
      for (int j = 0; j < W_NUM; j++){
        (*w_)[i][j] = w_temp[i][j];
      }
    }
  }

  *sum_loss = *sum_loss + loss_;
}

void predict_label(log_type *log, double *wx, long count){
  int j = 0;
  double val = wx[0];
  for(int i = 1; i < O_NUM; i++){
    if(wx[i] > val){
      val = wx[i];
      j = i;
    }
  }

  if(j == 0){
    log->node_label[count] = VEG;
  }else if(j == 1){
    log->node_label[count] = WIRE;
  }else if(j == 2){
    log->node_label[count] = POLE;
  }else if(j == 3){
    log->node_label[count] = GROUND;
  }else if(j == 4){
    log->node_label[count] = FACADE;
  }    
}

void Gradient_Descent(log_type train_log, log_type test_log, log_type *gradient_log_online, log_type *gradient_log_stat){
  int y[O_NUM] = {0}, y_[O_NUM] = {0}, type;            
  double x[W_NUM], wx[O_NUM], wx_[O_NUM], dl[O_NUM][W_NUM], alpha; 
  double w[O_NUM][W_NUM], w_[O_NUM][W_NUM], w_store[O_NUM][W_NUM][O_NUM];
  double regret[train_log.count], sum_loss = 0, sum_loss_ = 0;

  /*********** Online Learning ***********/
  alpha = 0.01;
  gradient_log_online->count = train_log.count;  
  new_hornetsoft_log(gradient_log_online);

  for(int i = 0; i < O_NUM; i++){ //initializa weight
    for(int j = 0; j < W_NUM; j++){
      for(int k = 0; k < O_NUM; k++){
        w_store[i][j][k] = 0.0;
      }
      w[i][j] = 0.0;
      w_[i][j] = 0.0;
    }
  }

  for(long i = 0; i < train_log.count; i++){
    gradient_log_online->point[i] = train_log.point[i];
    gradient_log_online->node_id[i] = train_log.node_id[i];

    assign_weight(y, x, &w, &w_, &w_store, &sum_loss, type);
    multiply_vectors(wx_, &w_, x);
    predict_label(gradient_log_online, wx_, i); //predict the output label with the best set of weights

    assign_output(train_log, i, y, &type);
    assign_input(train_log, i, x);
    multiply_vectors(wx, &w, x);

    update_gradient(&dl, wx, x, y);    
    update_weight(&dl, &w, alpha);
  }

  /*********** Statistical Learning ***********/
  alpha = 0.000001; //decrease step size since we can repeat the data several times
  log_type train_log_sort;
  sort_log(train_log, &train_log_sort);

  for(int i = 0; i < O_NUM; i++){ //re-initializa weight
    for(int j = 0; j < W_NUM; j++){
      w[i][j] = 1.0;
    }
  }

  /* Training */
  for(int k = 0; k < EPOCH_NUM; k++){
    for(long i = 0; i < train_log_sort.count; i++){
      assign_output(train_log_sort, i, y, &type);
      assign_input(train_log_sort, i, x);
      multiply_vectors(wx, &w, x);
      update_gradient(&dl, wx, x, y);
      update_weight(&dl, &w, alpha);
    }
  }

  /* Testing */
  gradient_log_stat->count = test_log.count;  
  new_hornetsoft_log(gradient_log_stat);
  
  for(long i = 0; i < test_log.count; i++){
    gradient_log_stat->point[i] = test_log.point[i];
    gradient_log_stat->node_id[i] = test_log.node_id[i];

    assign_input(test_log, i, x);
    multiply_vectors(wx, &w, x); //use latest weight
    predict_label(gradient_log_stat, wx, i);
  }

  return regret[train_log.count - 1];
}

// if(i > -1){
//           printf("\n");
//           // for(int i = 0; i < O_NUM; i++){
//           //   for (int j = 0; j < W_NUM; j++){
//           //     printf("%.4f ", w[i][j]);
//           //   }
//           //   printf("\n");
//           // }
//           for(int j = 0; j < O_NUM; j++){
//             printf("%.4f ", wx[j]);
//           }
//           printf("\n");
//           for(int j = 0; j < O_NUM; j++){
//             printf("%d ", y[j]);
//           }
//         }