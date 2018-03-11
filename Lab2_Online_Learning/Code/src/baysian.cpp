#include "baysian.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>
#include <Eigen/Dense>

#define S 1 //Guassian Noise
#define LOOP_NUM 1

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
			m[i] = A(i,j)*J[j];
		}
	}
	// for(int i = 0; i < W_NUM; i++){
	// 	for(int j = 0; j < W_NUM; j++){
	// 		(*V)[i][j] = A(i,j);
	// 	}
	// }
	// for(int i = 0; i < W_NUM; i++){
	// 	for(int j = 0; j < W_NUM; j++){
	// 		m[i] = (*V)[i][j]*J[j];
	// 	}
	// }
	// normal_random_variable sample { A.inverse() };
	// sampling = sample();
	// for(int i = 0; i < W_NUM; i++){
	// 	w[i] = sampling(i) + m[i];
	// }
}

void predict_output(double *w, double *x, double *y, int k){
	y[k] = 0;
	for(int i = 0; i < W_NUM; i++){
		y[k] = y[k] + w[i]*x[i];
	}
}

void BLR_predict_label(log_type *log, double *y, long count){
	for(int k = 0; k < O_NUM; k++){
  		y[k] = fabs(y[k]);
  	}

  	if(y[0] > 0.5){
  		log->node_label[count] = VEG;
  	}else if(y[1] > 0.5){
  		log->node_label[count] = WIRE;
  	}else if(y[2] > 0.5){
  		log->node_label[count] = POLE;
  	}else if(y[4] > 0.5){
  		log->node_label[count] = FACADE;
  	}else{
  		log->node_label[count] = GROUND;
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

  	/*********** Online Learning ***********/
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

  	BLR_log_online->count = train_log.count;  
  	new_hornetsoft_log(BLR_log_online);

  	for(long i = 0; i < train_log.count; i++){
  		BLR_log_online->point[i] = train_log.point[i];
    	BLR_log_online->node_id[i] = train_log.node_id[i];
  		assign_output(train_log, i, y, &type);
  		assign_input(train_log, i, x);

  		for(int k = 0; k < O_NUM; k++){ //Classify one-vs-all for 5 times
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
  		BLR_predict_label(BLR_log_online, y_, i);
  	}

  	/*********** Statistical Learning ***********/

  	// for(int i = 0; i < W_NUM; i++){ //re-initializa weight
   //  	for(int j = 0; j < W_NUM; j++){
   //  		if(i == j){
   //    			P1[i][j] = 1.0;
   //    			P2[i][j] = 1.0;
   //    			P3[i][j] = 1.0;
   //    			P4[i][j] = 1.0;
   //    			P5[i][j] = 1.0;
   //    		}
   //  	}
   //  	J1[i] = 0.0;
   //  	J2[i] = 0.0;
   //  	J3[i] = 0.0;
   //  	J4[i] = 0.0;
   //  	J5[i] = 0.0;
  	// }

  	// log_type train_log_sort;
  	// sort_log(train_log, &train_log_sort);
  	
  	// /* Training */
  	// for(int n = 0; n < LOOP_NUM; n++){
  	// 	for(long i = 0; i < train_log_sort.count; i++){
  	// 		assign_output(train_log_sort, i, y, &type);
  	// 		assign_input(train_log_sort, i, x);

  	// 		for(int k = 0; k < O_NUM; k++){ //Classify one-vs-all for 5 times
  	// 			if(k == 0){
  	// 				update_J(J1, y, x, k);
  	// 				update_P(&P1, x);  	
  	// 			}else if(k == 1){
  	// 				update_J(J2, y, x, k);
  	// 				update_P(&P2, x);  	
  	// 			}else if(k == 2){
  	// 				update_J(J3, y, x, k);
  	// 				update_P(&P3, x);  	
  	// 			}else if(k == 3){
  	// 				update_J(J4, y, x, k);
  	// 				update_P(&P4, x);  	
  	// 			}else if(k == 4){
  	// 				update_J(J5, y, x, k);
  	// 				update_P(&P5, x);
  	// 			}
  	// 		}
  	// 	}
  	// }

  	// /* Testing */
  	// BLR_log_stat->count = test_log.count;  
  	// new_hornetsoft_log(BLR_log_stat);
  	// for(int i = 0; i < test_log.count; i++){
  	// 	BLR_log_stat->point[i] = test_log.point[i];
   //  	BLR_log_stat->node_id[i] = test_log.node_id[i];
   //  	assign_input(test_log, i, x);
   //  	assign_output(test_log, i, y, &type);
   //  	for(int k = 0; k < O_NUM; k++){ //Classify one-vs-all for 5 times
  	// 		if(k == 0){
  	// 			get_weight(&V1, m1, &P1, J1, w1);
  	// 			predict_output(m1, x, y_, k);
  	// 		}else if(k == 1){
  	// 			get_weight(&V2, m2, &P2, J2, w2);
  	// 			predict_output(m2, x, y_, k);
  	// 		}else if(k == 2){
  	// 			get_weight(&V3, m3, &P3, J3, w3);
  	// 			predict_output(m3, x, y_, k);
  	// 		}else if(k == 3){
  	// 			get_weight(&V4, m4, &P4, J4, w4);
  	// 			predict_output(m4, x, y_, k); 	
  	// 		}else if(k == 4){
  	// 			get_weight(&V5, m5, &P5, J5, w5);
  	// 			predict_output(m5, x, y_, k);
  	// 		}
  	// 	}
  	// 	for(int k = 0; k < O_NUM; k++){
  	// 		printf("%.4f %d\n", y_[k], y[k]);
  	// 	}
  	// 	printf("\n");
  	// 	BLR_predict_label(BLR_log_stat, y_, i);
  	// }  
}