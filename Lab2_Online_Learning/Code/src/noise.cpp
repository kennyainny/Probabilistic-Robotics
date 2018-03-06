#include "noise.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>

using namespace std;
random_device rd_u, rd_d;
mt19937 mt_u(rd_u()), mt_d(rd_d());

void assign_input_noise(log_type log, long count, double *x){
	x[0] = log.point[count].x;
	x[1] = log.point[count].y;
	x[2] = log.point[count].z;
	for(int j = 0; j < 9; j++){
		x[j+3] = log.feature[count].f[j];
	}
}

void assign_feature_noise(log_type *log, long count, double *f, int y){
	log->point[count].x = f[0];
	log->point[count].y = f[1];
	log->point[count].z = f[2];
	for(int j = 0; j < 9; j++){
		log->feature[count].f[j] = f[3+j];
	}
	log->node_label[count] = y;
}

void Add_Noise_1(log_type log, log_type *log_noise){
	int feature_size = 12; //x y z  and 9 features
	int y; //node label
	double f[feature_size];
	double min_f[feature_size] = {999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999}; 
	double max_f[feature_size] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	for(long i = 0; i < log.count; i++){
		assign_input_noise(log, i, f);
		for(int j = 0; j < feature_size; j++){
			if(f[j] < min_f[j])
				min_f[j] = f[j];
			if(f[j] > max_f[j])
				max_f[j] = f[j];
		}
	}

	long noise_size = NOISE_RATIO*log.count;
	log_noise->count = log.count + noise_size;
	new_hornetsoft_log(log_noise);

	uniform_real_distribution<float> distr_u;
	discrete_distribution<int> distr_d {VEG, WIRE, POLE, GROUND, FACADE};
	for(long i = 0; i < log_noise->count; i++){
		log_noise->node_id[i] = i;
		if(i < log.count){
			log_noise->point = log.point;
			log_noise->node_label = log.node_label;
			log_noise->feature = log.feature;
		}else{
			for(int j = 0; j < feature_size; j++){
				distr_u.param(uniform_real_distribution<float>::param_type(min_f[j], max_f[j]));
				f[j] = distr_u(mt_u);
			}
			y = distr_d(mt_d);
			assign_feature_noise(log_noise, i, f, y);
		}
	}
}

void Add_Noise_2(log_type log, log_type *log_noise){

}