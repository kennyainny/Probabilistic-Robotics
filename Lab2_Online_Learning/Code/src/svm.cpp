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

void predict_output(double *w, double *x, double *y){
	*y = 0;
	for(int i = 0; i < W_NUM; i++){
		*y = *y + w[i]*x[i];
	}
}

void BLR(log_type train_log, log_type test_log, log_type *BLR_log_online, log_type *BLR_log_stat){
	int y[O_NUM], type;
	double x[W_NUM], w[W_NUM], y_;
	double J[W_NUM], P[W_NUM][W_NUM], V[W_NUM][W_NUM], m[W_NUM];
	normal_distribution<float> distr;

	for(int i = 0; i < W_NUM; i++){ //initializa weight
    	for(int j = 0; j < W_NUM; j++){
    		if(i == j){
      			P[i][j] = 1.0;
      		}
    	}
    	J[i] = 0.0;
  	}

  	/*********** Online Learning ***********/
  	BLR_log_online->count = train_log.count;  
  	new_hornetsoft_log(BLR_log_online);
  	for(long i = 0; i < BLR_log_online->count; i++){
  		BLR_log_online->point[i] = train_log.point[i];
    	BLR_log_online->node_id[i] = train_log.node_id[i];
  	}

  	for(int k = 0; k < 1; k++){ //Classify one-vs-all
  		for(long i = 0; i < train_log.count; i++){
  			assign_output(train_log, i, y, &type);
  			assign_input(train_log, i, x);

  			get_weight(&V, m, &P, J, w);
  			predict_output(m, x, &y_);

  			update_J(J, y, x, k);
  			update_P(&P, x);  			

  			// for(int a = 0; a < W_NUM; a++){ //initialize weight
    	// 		for(int b = 0; b < W_NUM; b++){
     //  				printf("%.2f ", P[a][b]);
    	// 		}
    	// 		printf("\n");
    	// 	}
    	// 	printf("m ");
    	// 	for(int a = 0; a < W_NUM; a++){
    	// 		printf("%.2f ", m[a]);
  			// }
  			// printf("\nx ");
  			// for(int a = 0; a < W_NUM; a++){
    	// 		printf("%.2f ", x[a]);
  			// }
  			// printf("\n");
  			// printf("%lu %d %d %d %.2f\n", i, k, type, y[k], y_);
  			// printf("\n");
  			if(y_ > 0.5){
  				BLR_log_online->node_label[i] = VEG;
  			}else{
  				BLR_log_online->node_label[i] = FACADE;
  			}
  		}
  	}
}