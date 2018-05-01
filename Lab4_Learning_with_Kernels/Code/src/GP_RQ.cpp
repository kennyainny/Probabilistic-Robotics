#include "GP_RQ.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <Eigen/LU>
#include <Eigen/QR>

using namespace std;
using namespace Eigen;

#define L 1
#define ALPHA 2

double K_RQ(feature_type x1, feature_type x2, double l, double alpha){
  double r2 = 0, k = 0;
  for(int i = 0; i < 9; i++){
    r2 = r2 + (x1.f[i] - x2.f[i])*(x1.f[i] - x2.f[i]);
  }
  k = 1/pow(1+r2/2/alpha/l/l, alpha);
  // k = sqrt(1+r2);
  return k;
}

void GP_RQ(log_type train_log, log_type test_log, log_type *GP_RQ_Train, log_type *GP_RQ_Test){
  GP_RQ_Train->count = train_log.count;  
  new_hornetsoft_log(GP_RQ_Train);
  GP_RQ_Test->count = test_log.count;  
  new_hornetsoft_log(GP_RQ_Test);

  MatrixXd K_xtr_xtr(train_log.count, train_log.count);
  MatrixXd K_x_xtr(test_log.count, train_log.count); 
  MatrixXd K_xtr_x(train_log.count, test_log.count);  
  MatrixXd K_x_x(test_log.count, test_log.count);
  MatrixXd K(test_log.count, train_log.count);
  MatrixXd Y(train_log.count, 2);
  MatrixXd Y_test(test_log.count, 2);
  MatrixXd mu(test_log.count, 2);

  for(int i = 0; i < train_log.count; i++){
    for(int j = 0; j < train_log.count; j++){
      K_xtr_xtr(i,j) = K_RQ(train_log.feature[i], train_log.feature[j], L, ALPHA);
    }

    if(train_log.node_label[i] == LABEL1){
      Y(i,0) = 1;
      Y(i,1) = 0;
    }else if(train_log.node_label[i] == LABEL2){
      Y(i,0) = 0;
      Y(i,1) = 1;
    }

  }

  for(int i = 0; i < test_log.count; i++){
    for(int j = 0; j < train_log.count; j++){
      K_x_xtr(i,j) = K_RQ(test_log.feature[i], train_log.feature[j], L, ALPHA);
    }

    if(test_log.node_label[i] == LABEL1){
      Y_test(i,0) = 1;
      Y_test(i,1) = 0;
    }else if(test_log.node_label[i] == LABEL2){
      Y_test(i,0) = 0;
      Y_test(i,1) = 1;
    }
  }  

  // for(int i = 0; i < train_log.count; i++){
  //   for(int j = 0; j < test_log.count; j++){
  //     K_xtr_x(i,j) = K_RBF(train_log.feature[i], test_log.feature[j], L, ALPHA);
  //   }
  // }  

  // for(int i = 0; i < test_log.count; i++){
  //   for(int j = 0; j < test_log.count; j++){
  //     K_x_x(i,j) = K_RBF(test_log.feature[i], test_log.feature[j], L, ALPHA);
  //   }
  // }

  // Matrix3d A;
  // int x[3] = {-2,1,2};
  // for(int i = 0; i < 3; i++){
  //   for(int j = 0; j < 3; j++){
  //     A(i,j) = K_RBF2(x[i], x[j], L, ALPHA);
  //   }
  // }
  // cout << pinv(A) << endl;

  // MatrixXd A(3,6);
  // A.setRandom();
  HouseholderQR<MatrixXd> qr(K_xtr_xtr.transpose());
  MatrixXd pinv;
  pinv.setIdentity(K_xtr_xtr.cols(), K_xtr_xtr.rows());
  pinv = qr.householderQ() * pinv;
  pinv = qr.matrixQR().topLeftCorner(K_xtr_xtr.rows(),K_xtr_xtr.rows()).triangularView<Upper>().transpose().solve<OnTheRight>(pinv);
  // cout << pinv << endl;
  mu = K_x_xtr*pinv*Y;

  for(int i = 0; i < test_log.count; i++){
    cout << mu(i,0) << ", " << mu(i,1) << ", " << Y_test(i,0) << ", " << Y_test(i,1) << endl;

    GP_RQ_Test->point[i] = test_log.point[i];

    if(mu(i,0) > mu(i,1)){
      GP_RQ_Test->node_label[i] = LABEL1;
    }else{
      GP_RQ_Test->node_label[i] = LABEL2;
    }
  }
}