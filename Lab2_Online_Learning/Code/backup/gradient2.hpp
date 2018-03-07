#ifndef GRADIENT_H
#define GRADIENT_H

#define O_NUM 5
#define W_NUM 12

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"

void Gradient_Descent(log_type train_log, log_type test_log, log_type *gradient_log);
void assign_output(log_type log, long count, int *y, int *y_ind);
void assign_input(log_type log, long count, double *x);
void assign_weight(int y, double *wx, double *err, double (*w)[O_NUM][W_NUM], double (*w_)[O_NUM][W_NUM], long count, int y_ind);
void update_gradient(log_type log, double (*dl)[O_NUM][W_NUM], double *wx, double *x, int y, long count, int y_ind);
void update_weight(double (*dl)[O_NUM][W_NUM], double (*w)[O_NUM][W_NUM], int y_ind);
void multiply_vectors(double *wx, double (*w)[O_NUM][W_NUM], double *x, int y_ind);
void assign_result(double *wx, int *y_, int y_ind, double *wx_);

#endif /* GRADIENT_H */
