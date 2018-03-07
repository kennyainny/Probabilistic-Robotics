#ifndef GRADIENT_H
#define GRADIENT_H

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"

void Gradient_Descent(log_type train_log, log_type test_log, log_type *gradient_log);
double assign_output(log_type log, long count);
void assign_input(log_type log, long count, double *x);
void assign_min_max(log_type log, double *min_x, double *max_x, int weight_size);
void assign_weight(double y, double wx, double *err, double *w, double *w_, int weight_size, long count);
void update_gradient(log_type log, double *dl, double *w, double *x, double y, int weight_size, long count);
void update_weight(double *dl, double *w, int weight_size);
double multiply_vectors(double *w, double *x, int weight_size);

#endif /* GRADIENT_H */
