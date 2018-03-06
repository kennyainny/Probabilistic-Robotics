#include "noise.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>

using namespace std;
random_device rd_u, rd_d, rd_n;
mt19937 mt_u(rd_u()), mt_d(rd_d()), mt_n(rd_n());

void assign_output_noise(int *y){
	if(*y == 0)
		*y = VEG;
	else if(*y == 1)
		*y = WIRE;
	else if(*y == 2)
		*y = POLE;
	else if(*y == 3)
		*y = GROUND;
	else if(*y == 4)
		*y = FACADE;
}

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
	log->feature[count].bias = 1;
	log->node_label[count] = y;
}

void calculate_mean(log_type log, double *mean, int size){
	double f[size], sum[size] = {0};
	for(int i = 0; i < log.count; i++){
		assign_input_noise(log, i, f);
		for(int j = 0; j < size; j++){
			sum[j] = sum[j] + f[j];
		}
	}

	for(int j = 0; j < size; j++){
		mean[j] = sum[j]/log.count;
	}
}

void calculate_variance(log_type log, double *mean, double *var, int size){
	double f[size], sum[size] = {0};
	for(int i = 0; i < log.count; i++){
		assign_input_noise(log, i, f);
		for(int j = 0; j < size; j++){
			sum[j] = sum[j] + pow(f[j], 2);
		}
	}

	for(int j = 0; j < size; j++){
		var[j] = sum[j]/log.count - pow(mean[j], 2);
	}
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
			log_noise->point[i] = log.point[i];
			log_noise->node_label[i] = log.node_label[i];
			log_noise->feature[i] = log.feature[i];
		}else{			
			for(int j = 0; j < feature_size; j++){
				distr_u.param(uniform_real_distribution<float>::param_type(min_f[j], max_f[j]));
				f[j] = distr_u(mt_u);
			}
			y = distr_d(mt_d);
			assign_output_noise(&y);
			assign_feature_noise(log_noise, i, f, y);
		}
	}
}

void Add_Noise_2(log_type log, log_type *log_noise){
	int feature_size = 12;
	double mean[12], var[12];
	calculate_mean(log, mean, feature_size);
	calculate_variance(log, mean, var, feature_size);

	long noise_size = NOISE_RATIO*log.count;
	log_noise->count = log.count + noise_size;
	new_hornetsoft_log(log_noise);

	//uniformly pick points to be corrupted
	uniform_int_distribution<float> distr_u(0, log.count-1);
	long corrupted_node_id[noise_size];
	for(int i = 0; i < noise_size; i++){
		corrupted_node_id[i] = distr_u(mt_u);
	}

	//corrupting those points by gaussian noise
}