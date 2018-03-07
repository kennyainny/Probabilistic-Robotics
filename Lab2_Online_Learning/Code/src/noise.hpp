#ifndef NOISE_H
#define NOISE_H

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"

void assign_output_noise(int *y);
void assign_input_noise(log_type log, long count, double *x);
void assign_feature_noise(log_type *log, long count, double *f, int y);
void calculate_mean(log_type log, double *mean, int size);
void calculate_variance(log_type log, double *mean, double *var, int size);
void Add_Noise_1(log_type log, log_type *log_noise);
void Add_Noise_2(log_type log, log_type *log_noise);

#endif /* NOISE_H */
