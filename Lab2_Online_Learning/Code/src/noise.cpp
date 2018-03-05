#include "noise.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>

using namespace std;

void Add_Noise_1(log_type log, log_type *log_noise){
	int feature_size = 12; //x y z  and 9 features
	int y; //node label
	double f[feature_size];
	double min_f[feature_size] = {999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999}; 
	double max_f[feature_size] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	for(long i = 0; i < log.count; i++){
		assign_input(log, i, f);
		for(int j = 0; j < feature_size; j++){
			if(f[j] < min_f[j])
				min_f[j] = f[j];
			if(f[j] > max_f)
				max_f[j] = f[j];
		}
	}

	long noise_size = NOISE_RATIO*log.count;
	log_noise->count = log.count + noise_size;
	new_hornetsoft_log(log_noise);

	for(long i = 0; i < log_noise->count; i++){
		if(i < log.count){
			log_noise->point = log.point;
			log_noise->node_id = log.node_id;
			log_noise->node_label = log.node_label;
			log_noise->feature = log.feature;
		}else{

		}
	}
}

void Add_Noise_2(log_type log, log_type *log_noise){

}