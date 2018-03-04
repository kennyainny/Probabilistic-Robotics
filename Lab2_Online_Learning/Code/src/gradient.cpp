#include "gradient.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ALPHA 0.5

#define Y_VEG    10000
#define Y_WIRE   1000
#define Y_POLE   100
#define Y_GROUND 10
#define Y_FACADE 1

#define EPOCH_NUM 200

void Gradient_Descent(log_type train_log, log_type test_log, log_type *gradient_log){
  gradient_log->count = test_log.count;  
  new_hornetsoft_log(gradient_log);
  gradient_log->point = test_log.point;
  gradient_log->node_id = test_log.node_id;
  double w[12] = {1,1,1,1,1,1,1,1,1,1,1,1};
  double wx, l, dl[12], DL, x[12];
  double err_veg = 9999999, err_wire = 9999999, err_pole = 9999999, err_ground = 9999999, err_facade = 9999999;
  double w_veg[12], w_wire[12], w_pole[12], w_ground[12], w_facade[12];
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

    x[0] = train_log.point[i].x;
    x[1] = train_log.point[i].y;
    x[2] = train_log.point[i].z;
    for(int j = 0; j < 9; j++){
      x[j+3] = train_log.feature[i].f[j];
    }

    wx = 0, DL = 0;
    for (int j = 0; j < 12; j++)
    {
      wx = wx + w[j]*x[j];
      // if(k == EPOCH_NUM-1)
      // printf("%d: x %.4f wx %.4f w %.4f\n",j+1 , x[j], wx, w[j]);
    }

    l = pow(wx - y, 2.0);
    // printf("l: %.4f\n", l);

    for (int j = 0; j < 12; j++)
    {
      dl[j] = 2*(wx + train_log.feature[i].bias - y)*x[j];
      DL = DL + pow(dl[j], 2.0);
    }
    DL = sqrt(DL);

    wx = 0;
    for (int j = 0; j < 12; j++)
    { 
      w[j] = w[j] - ALPHA*dl[j]/DL;
      wx = wx + w[j]*x[j];
    }

    if(k == EPOCH_NUM - 1){
      // printf("step: %d %d %.4f %.4f \n", i, y, wx, w[0]);
      if(y == Y_VEG){
        if(abs(y - wx) < err_veg){
          err_veg = abs(y - wx);
          for(int j = 0; j < 12; j++)
            w_veg[j] = w[j];
        }
      }else if(y == Y_WIRE){
        if(abs(y - wx) < err_wire){
          err_wire = abs(y - wx);
          for(int j = 0; j < 12; j++)
            w_wire[j] = w[j];
        }
      }else if(y == Y_POLE){
        if(abs(y - wx) < err_pole){
          err_pole = abs(y - wx);
          for(int j = 0; j < 12; j++)
            w_pole[j] = w[j];
        }
      }else if(y == Y_GROUND){
        if(abs(y - wx) < err_ground){
          err_ground = abs(y - wx);
          for(int j = 0; j < 12; j++)
            w_ground[j] = w[j];
        }
      }else if(y == Y_FACADE){
        if(abs(y - wx) < err_facade){
          err_facade = abs(y - wx);
          for(int j = 0; j < 12; j++)
            w_facade[j] = w[j];
        }
      }
    }
  }
}

  // for(int j = 0; j < 12; j++){
  //   printf("%d %.4f %.4f %.4f %.4f %.4f\n", j, w_veg[j], w_wire[j], w_pole[j], w_ground[j], w_facade[j]);
  // }

  for(int i = 0; i < test_log.count; i++){
    if(test_log.node_label[i] == VEG){
      y = Y_VEG;
    }else if(test_log.node_label[i] == WIRE){
      y = Y_WIRE;
    }else if(test_log.node_label[i] == POLE){
      y = Y_POLE;
    }else if(test_log.node_label[i] == GROUND){
      y = Y_GROUND;
    }else if(test_log.node_label[i] == FACADE){
      y = Y_FACADE;
    }

    x[0] = test_log.point[i].x;
    x[1] = test_log.point[i].y;
    x[2] = test_log.point[i].z;
    for(int j = 0; j < 9; j++){
      x[j+3] = test_log.feature[i].f[j];
    }

    wx = 0;
    for (int j = 0; j < 12; j++)
    {
      if(y == Y_VEG){
        wx = wx + w_veg[j]*x[j];
      }else if(y == Y_WIRE){
        wx = wx + w_wire[j]*x[j];
      }else if(y == Y_POLE){
        wx = wx + w_pole[j]*x[j];
      }else if(y == Y_GROUND){
        wx = wx + w_ground[j]*x[j];
      }else if(y == Y_FACADE){
        wx = wx + w_facade[j]*x[j];
      }      
    }

    // if(wx < Y_FACADE){
    //   gradient_log->node_label[i] = Y_FACADE;
    // }else if(wx > Y_FACADE && wx < Y_GROUND){
    //   if(abs(wx - Y_FACADE) < abs(wx - Y_GROUND)){
    //     gradient_log->node_label[i] = Y_FACADE;
    //   }else{
    //     gradient_log->node_label[i] = Y_GROUND;
    //   }      
    // }else if(wx > Y_GROUND && wx < Y_POLE){
    //   if(abs(wx - Y_GROUND) < abs(wx - Y_POLE)){
    //     gradient_log->node_label[i] = Y_GROUND;
    //   }else{
    //     gradient_log->node_label[i] = Y_POLE;
    //   }  
    // }else if(wx > Y_POLE && wx < Y_WIRE){
    //   if(abs(wx - Y_POLE) < abs(wx - Y_WIRE)){
    //     gradient_log->node_label[i] = Y_POLE;
    //   }else{
    //     gradient_log->node_label[i] = Y_WIRE;
    //   }  
    // }else if(wx > Y_WIRE && wx < Y_VEG){
    //   if(abs(wx - Y_WIRE) < abs(wx - Y_VEG)){
    //     gradient_log->node_label[i] = Y_WIRE;
    //   }else{
    //     gradient_log->node_label[i] = Y_VEG;
    //   }  
    // }else if(wx > Y_VEG){
    //   gradient_log->node_label[i] = Y_VEG;
    // }

    // printf("step: %d %d %.4f %d\n", i, y, wx, gradient_log->node_label[i]);

    if(wx < Y_FACADE){
      gradient_log->node_label[i] = FACADE;
    }else if(wx > Y_FACADE && wx < Y_GROUND){
      if(abs(wx - Y_FACADE) < abs(wx - Y_GROUND)){
        gradient_log->node_label[i] = FACADE;
      }else{
        gradient_log->node_label[i] = GROUND;
      }      
    }else if(wx > Y_GROUND && wx < Y_POLE){
      if(abs(wx - Y_GROUND) < abs(wx - Y_POLE)){
        gradient_log->node_label[i] = GROUND;
      }else{
        gradient_log->node_label[i] = POLE;
      }  
    }else if(wx > Y_POLE && wx < Y_WIRE){
      if(abs(wx - Y_POLE) < abs(wx - Y_WIRE)){
        gradient_log->node_label[i] = POLE;
      }else{
        gradient_log->node_label[i] = WIRE;
      }  
    }else if(wx > Y_WIRE && wx < Y_VEG){
      if(abs(wx - Y_WIRE) < abs(wx - Y_VEG)){
        gradient_log->node_label[i] = WIRE;
      }else{
        gradient_log->node_label[i] = VEG;
      }  
    }else if(wx > Y_VEG){
      gradient_log->node_label[i] = VEG;
    }    
  }
}

using namespace std;