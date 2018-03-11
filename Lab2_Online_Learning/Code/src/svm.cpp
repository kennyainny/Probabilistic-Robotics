#include "svm.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>
#include <Eigen/Dense>

#define S 1 //Guassian Noise

using namespace std;
using namespace Eigen;

random_device rd;
mt19937 mt(rd());


struct normal_random_variable
{
    normal_random_variable(Eigen::MatrixXd const& covar)
        : normal_random_variable(Eigen::VectorXd::Zero(covar.rows()), covar)
    {}

    normal_random_variable(Eigen::VectorXd const& mean, Eigen::MatrixXd const& covar)
        : mean(mean)
    {
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigenSolver(covar);
        transform = eigenSolver.eigenvectors() * eigenSolver.eigenvalues().cwiseSqrt().asDiagonal();
    }

    Eigen::VectorXd mean;
    Eigen::MatrixXd transform;

    Eigen::VectorXd operator()() const
    {
        static std::mt19937 gen{ std::random_device{}() };
        static std::normal_distribution<> dist;

        return mean + transform * Eigen::VectorXd{ mean.size() }.unaryExpr([&](double x) { return dist(gen); });
    }
};

void update_J(double *J, int *y, double *x, int k){
	for(int i = 0; i < W_NUM; i++){
		J[i] = J[i] + 1/S/S*y[k]*x[i];
	}
}

void update_P(double (*P)[W_NUM][W_NUM], double *x){
	for(int i = 0; i < W_NUM; i++){ //initializa weight
    	for(int j = 0; j < W_NUM; j++){
    		(*P)[i][j] = (*P)[i][j] + 1/S/S*x[i]*x[j];
    	}
  	}
}

void get_weight(double (*V)[W_NUM][W_NUM], double *m, double (*P)[W_NUM][W_NUM], double *J, double *w){
	MatrixXd A(W_NUM,W_NUM);
	VectorXd sampling(W_NUM);
	for(int i = 0; i < W_NUM; i++){
		for(int j = 0; j < W_NUM; j++){
			A(i,j) = (*P)[i][j];
		}
	}
	A = A.inverse();
	for(int i = 0; i < W_NUM; i++){
		for(int j = 0; j < W_NUM; j++){
			(*V)[i][j] = A(i,j);
		}
	}
	for(int i = 0; i < W_NUM; i++){
		for(int j = 0; j < W_NUM; j++){
			m[i] = (*V)[i][j]*J[j];
		}
	}
	normal_random_variable sample { A.inverse() };
	sampling = sample();
	for(int i = 0; i < W_NUM; i++){
		w[i] = sampling(i) + m[i];
	}
}

void predict_output(double *w, double *x, double *y, int k){
	y[k] = 0;
	for(int i = 0; i < W_NUM; i++){
		y[k] = y[k] + w[i]*x[i];
	}
}

void BLR(log_type train_log, log_type test_log, log_type *BLR_log_online, log_type *BLR_log_stat){
	int y[O_NUM], type;
	double x[W_NUM], y_[W_NUM];
	double w1[W_NUM], J1[W_NUM], P1[W_NUM][W_NUM], V1[W_NUM][W_NUM], m1[W_NUM];
	double w2[W_NUM], J2[W_NUM], P2[W_NUM][W_NUM], V2[W_NUM][W_NUM], m2[W_NUM];
	double w3[W_NUM], J3[W_NUM], P3[W_NUM][W_NUM], V3[W_NUM][W_NUM], m3[W_NUM];
	double w4[W_NUM], J4[W_NUM], P4[W_NUM][W_NUM], V4[W_NUM][W_NUM], m4[W_NUM];
	double w5[W_NUM], J5[W_NUM], P5[W_NUM][W_NUM], V5[W_NUM][W_NUM], m5[W_NUM];
	normal_distribution<float> distr;

	for(int i = 0; i < W_NUM; i++){ //initializa weight
    	for(int j = 0; j < W_NUM; j++){
    		if(i == j){
      			P1[i][j] = 1.0;
      			P2[i][j] = 1.0;
      			P3[i][j] = 1.0;
      			P4[i][j] = 1.0;
      			P5[i][j] = 1.0;
      		}
    	}
    	J1[i] = 0.0;
    	J2[i] = 0.0;
    	J3[i] = 0.0;
    	J4[i] = 0.0;
    	J5[i] = 0.0;
  	}

  	/*********** Online Learning ***********/
  	BLR_log_online->count = train_log.count;  
  	new_hornetsoft_log(BLR_log_online);
  	for(long i = 0; i < BLR_log_online->count; i++){
  		BLR_log_online->point[i] = train_log.point[i];
    	BLR_log_online->node_id[i] = train_log.node_id[i];
  	}

  	for(long i = 0; i < train_log.count; i++){
  			assign_output(train_log, i, y, &type);
  			assign_input(train_log, i, x);

  		for(int k = 0; k < O_NUM; k++){ //Classify one-vs-all
  		// for(long i = 0; i < train_log.count; i++){
  		// 	assign_output(train_log, i, y, &type);
  		// 	assign_input(train_log, i, x);
  			if(k == 0){
  				get_weight(&V1, m1, &P1, J1, w1);
  				predict_output(m1, x, y_, k);

  				update_J(J1, y, x, k);
  				update_P(&P1, x);  	
  			}else if(k == 1){
  				get_weight(&V2, m2, &P2, J2, w2);
  				predict_output(m2, x, y_, k);

  				update_J(J2, y, x, k);
  				update_P(&P2, x);  	
  			}else if(k == 2){
  				get_weight(&V3, m3, &P3, J3, w3);
  				predict_output(m3, x, y_, k);

  				update_J(J3, y, x, k);
  				update_P(&P3, x);  	
  			}else if(k == 3){
  				get_weight(&V4, m4, &P4, J4, w4);
  				predict_output(m4, x, y_, k);

  				update_J(J4, y, x, k);
  				update_P(&P4, x);  	
  			}else if(k == 4){
  				get_weight(&V5, m5, &P5, J5, w5);
  				predict_output(m5, x, y_, k);

  				update_J(J5, y, x, k);
  				update_P(&P5, x);
  			}
  		}

  		for(int k = 0; k < O_NUM; k++){
  			y_[k] = fabs(y_[k]);
  			// printf("%d %.4f\n", y[k], y_[k]);
  		}
  		// printf("\n");

  		if(y_[0] > 0.5){
  			BLR_log_online->node_label[i] = VEG;
  		}else if(y_[1] > 0.5){
  			BLR_log_online->node_label[i] = WIRE;
  		}else if(y_[2] > 0.5){
  			BLR_log_online->node_label[i] = POLE;
  		}else if(y_[4] > 0.5){
  			BLR_log_online->node_label[i] = FACADE;
  		}else{
  			BLR_log_online->node_label[i] = GROUND;
  		}
  		

  		// predict_label(BLR_log_online, y_, i);

  			// for(int a = 0; a < W_NUM; a++){ //initialize weight
    	// 		for(int b = 0; b < W_NUM; b++){
     //  				printf("%.2f ", P1[a][b]);
    	// 		}
    	// 		printf("\n");
    	// 	}
    	// 	printf("m ");
    	// 	for(int a = 0; a < W_NUM; a++){
    	// 		printf("%.2f ", m1[a]);
  			// }
  			// printf("\nx ");
  			// for(int a = 0; a < W_NUM; a++){
    	// 		printf("%.2f ", x[a]);
  			// }
  			// printf("\n");
  			// printf("%lu %d %d %.2f\n", i, type, y[0], y_[0]);
  			// printf("\n");

  			// if(y_ > 0.5){
  			// 	BLR_log_online->node_label[i] = FACADE;
  			// }else{
  			// 	BLR_log_online->node_label[i] = GROUND;
  			// }
  		
  	}
}