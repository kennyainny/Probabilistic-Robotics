#include "baysian.hpp"
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <random>
// #include <Eigen/Dense>

// #include "GP_RBF.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <Eigen/LU>
#include <Eigen/QR>
#include <vector>

using namespace std;
using namespace Eigen;

#define L 1
#define SIGMA 0.5
#define eta 0.05 //learning rate
#define lamda 0.001 //0.5

// double K_RBF(feature_type x1, feature_type x2, double l, double sigma);
// void KLR_RBF(log_type train_log, log_type test_log, log_type *GP_RBF_Train, log_type *GP_RBF_Test);

double K_RBF0(feature_type x1, feature_type x2, double l, double sigma)
{
  double r2 = 0, k = 0;
  for(int i = 0; i < 9; i++)
  {
    r2 = r2 + (x1.f[i] - x2.f[i])*(x1.f[i] - x2.f[i]);
  }
  k = sigma*sigma*exp(-r2/2/l/l);
  return k;
}

// double K_RBF2(double x1, double x2, double l, double sigma){
//   double r2 = 0, k = 0;
//   r2 = (x1 - x2)*(x1 - x2);
//   k = sigma*sigma*exp(-r2/2/l/l);
//   return k;
// }


void KLR_RBF(log_type train_log, log_type test_log, log_type *KLR_Train, log_type *KLR_Test)
{
  KLR_Train->count = train_log.count;  
  new_hornetsoft_log(KLR_Train);
  KLR_Test->count = test_log.count;  
  new_hornetsoft_log(KLR_Test);

  printf("Initializing Matrices\n");

  double len = train_log.count*1.0;

  MatrixXd K_xtr_xtr(train_log.count, train_log.count);
  MatrixXd K_x_xtr(test_log.count, train_log.count); 
  MatrixXd K_xtr_x(train_log.count, test_log.count);  
  MatrixXd K_x_x(test_log.count, test_log.count);
  MatrixXd K(test_log.count, train_log.count);
  
  vector<int> Y(train_log.count,0);
  vector<int> Y_test(test_log.count,0);

  vector<double>  _alpha(train_log.count, 1); //is this 1 D array?

  //build kernel
  printf("Bulding Kernel\n");
  for(int i = 0; i < train_log.count; i++)
  {
    for(int j = 0; j < train_log.count; j++)
    {
      K_xtr_xtr(i,j) = K_RBF0(train_log.feature[i], train_log.feature[j], L, SIGMA);
    }

    if(train_log.node_label[i] == LABEL1) //LABEL1 = +1
    {
      Y.at(i) = 1;
      // Y(i,1) = 0;
    }
    else if(train_log.node_label[i] == LABEL2) //LABEL1 = -1
    {
      Y.at(i) = -1;
      // Y(i,1) = 1;
    }
  }

  int xx = train_log.count - 1;
  
  // printf("Kernel: %f, %f\t Y: %d, %d\n", 
  // K_xtr_xtr(0,0), K_xtr_xtr(500,1000), Y[0], Y[700]);

  //init alpha
  // printf("init alpha\n");
  // for(int j = 0; j < train_log.count; j++)
  //   _alpha[j] = 0;

  //learn alpha iteratively
  printf("learning alpha\n");
  double delta, maxdelta =1;
  int cct = 0;

  while(cct < 50)
  {
    for(int i = 0; i < train_log.count; i++)
    {
        double fxy = 0.0;

        for(int j = 0; j < train_log.count; j++) //jth row
          fxy = fxy + K_xtr_xtr(i,j) * _alpha[j]; //ith column

        double _gamma = (Y[i]/(1.0 + exp(-Y[i]*fxy)))/(1.0*len);     

        for(int j = 0; j < train_log.count; j++)
        {
          delta = lamda*_alpha[j] + _gamma;
          double alp = _alpha[j] - eta*delta;
          _alpha[j] = alp;

          if(abs(delta) < maxdelta)
          maxdelta = delta;
        }
    }

    cct++;
    // printf("maxdelta: %f, cct: %d, alpha: %f\n",maxdelta,cct,_alpha[100]);
  }


  //K(X_T, x) - prediction/classification
  printf("Test Kernel\n");
  int a,b;
  for(int i = 0; i < test_log.count; i++)
  {
    for(int j = 0; j < train_log.count; j++)
    {
      K_x_xtr(i,j) = K_RBF0(test_log.feature[i], train_log.feature[j], L, SIGMA);
      b=j;
    }

    a=i; 
  }
    // printf("K_x_xtr: %f, i: %d, j: %d\n",K_x_xtr(20,100),a,b);

  //test learning
  printf("Output Test\n");
  // printf("K_x_xtr dimension: "); printf(K_x_xtr.size);

  for(int i = 0; i < test_log.count; i++)//ith column
  {
      double fxy = 0;

      for(int j = 0; j < train_log.count; j++) //jth row
        fxy = fxy + K_x_xtr(i,j) * _alpha[j]; 

      // double px = exp(fxy)/(1 + exp(fxy));
      // printf("fxy: %f, px: %f\n",fxy, px);

      KLR_Test->point[i] = test_log.point[i];

      if(fxy >= 0.5) 
      {
        Y_test.at(i) = 1;
        KLR_Test->node_label[i] = LABEL1;
      }
      else
      {
        Y_test.at(i) = -1;
        KLR_Test->node_label[i] = LABEL2;
      }
  }

  // HouseholderQR<MatrixXd> qr(K_xtr_xtr.transpose());
  // MatrixXd pinv;
  // pinv.setIdentity(K_xtr_xtr.cols(), K_xtr_xtr.rows());
  // pinv = qr.householderQ() * pinv;
  // pinv = qr.matrixQR().topLeftCorner(K_xtr_xtr.rows(),K_xtr_xtr.rows()).triangularView<Upper>().transpose().solve<OnTheRight>(pinv);
  // // cout << pinv << endl;
  // mu = K_x_xtr*pinv*Y;

    // for(int i = 0; i < test_log.count; i++)
    // {
    //   GP_RBF_Test->point[i] = test_log.point[i];

    //   if(mu(i,0) > mu(i,1))
    //   {
    //     GP_RBF_Test->node_label[i] = LABEL1;
    //   }
    //   else
    //   {
    //     GP_RBF_Test->node_label[i] = LABEL2;
    //   }
    // }
}