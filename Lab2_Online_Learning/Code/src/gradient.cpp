#include "gradient.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ALPHA 0.1

#define Y_VEG    1
#define Y_WIRE   10
#define Y_POLE   100
#define Y_GROUND 1000
#define Y_FACADE 10000

#define EPOCH_NUM 300

log_type Gradient_Descent(log_type train_log, log_type test_log){
  log_type gradient_log;
  gradient_log.count = test_log.count;
  new_hornetsoft_log(&gradient_log);
  double w[12] = {1,1,1,1,1,1,1,1,1,1,1,1};
  double wx, wn, l, dl[12], DL, x[12];
  int y;

  for(int k = 0; k <= EPOCH_NUM; k++){
  for(int i = 0; i < train_log.count; i++)
  {
    if(train_log.node_label[i] == VEG){
      y = Y_VEG;
    }else if(train_log.node_label[i] == WIRE){
      y = Y_WIRE;
    }else if(train_log.node_label[i] == POLE){
      y = Y_POLE;
    }else if(train_log.node_label[i] == GROUND){
      y = Y_GROUND;
    }else if(train_log.node_label[i] == FACADE){
      y = Y_FACADE;
    }
    // y = train_log.node_label[i];
    x[0] = train_log.point[i].x;
    x[1] = train_log.point[i].y;
    x[2] = train_log.point[i].z;
    for(int j = 0; j < 9; j++){
      x[j+3] = train_log.feature[i].f[j];
    }

    wx = 0, wn = 0, DL = 0;
    for (int j = 0; j < 12; j++)
    {
      wx = wx + w[j]*x[j];
      if(k == EPOCH_NUM-1)
      printf("%d: x %.4f wx %.4f w %.4f\n",j+1 , x[j], wx, w[j]);
    }

    l = pow(wx - y, 2.0);
    // printf("l: %.4f\n", l);

    for (int j = 0; j < 12; j++)
    {
      dl[j] = 2*(wx - y)*x[j];
      DL = DL + pow(dl[j], 2.0);
      // printf("dl%d: %.4f \n",j+1 , dl[j]);
    }
    DL = sqrt(DL);

    // printf("\n");
    // printf("DL: %.4f\n", DL);

    for (int j = 0; j < 12; j++)
    { 
      w[j] = w[j] - ALPHA*dl[j]/DL;
      // if(k == EPOCH_NUM-1)
      // printf("w%d: %f\n", j, w[j]);
    }

    if(k == EPOCH_NUM-1)
    printf("step: %d %d %.4f\n", i, y, wx);    
    // printf("\n");
  }
}

  for(int i = 0; i < train_log.count; i++){
    x[0] = train_log.point[i].x;
    x[1] = train_log.point[i].y;
    x[2] = train_log.point[i].z;
    for(int j = 0; j < 9; j++){
      x[j+3] = train_log.feature[i].f[j];
    }

    wx = 0;
    for (int j = 0; j < 12; j++)
    {
      wx = wx + w[j]*x[j];
      if(i == train_log.count-1 || i == 69183)
      printf("%d: %.4f %.4f %.4f \n",j+1 , w[j], x[j], wx);
    }

    // printf("wx: %.4f %d\n", wx, i);

    // if(l < THRES1){
    //   gradient_log.node_label[i] = WIRE;
    // }else if(l < THRES2){
    //   gradient_log.node_label[i] = VEG;
    // }else if(l < THRES3){
    //   gradient_log.node_label[i] = POLE;
    // }else if(l < THRES4){
    //   gradient_log.node_label[i] = GROUND;
    // }else{
    //   gradient_log.node_label[i] = FACADE;
    // }
  }
  // printf("wx: %.4f \n", wx);
  return gradient_log;
}

using namespace std;