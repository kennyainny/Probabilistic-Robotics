#ifndef GRADIENT_H
#define GRADIENT_H

#define O_NUM 5
#define W_NUM 10

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"

double Gradient_Descent(log_type train_log, log_type test_log, log_type *gradient_log_online, log_type *gradient_log_stat);
void assign_output(log_type log, long count, int *y, int *type);
void assign_input(log_type log, long count, double *x);
void assign_weight(int *y, double *wx, double *err, double (*w)[O_NUM][W_NUM], double (*w_)[O_NUM][W_NUM]);
void update_gradient(double (*dl)[O_NUM][W_NUM], double *wx, double *x, int *y);
void update_weight(double (*dl)[O_NUM][W_NUM], double (*w)[O_NUM][W_NUM], double alpha);
void multiply_vectors(double *wx, double (*w)[O_NUM][W_NUM], double *x);
void assign_result(double *wx, int *y_);
double loss_calc(double (*w)[O_NUM][W_NUM], double *x, int *y);
void assign_weight(int *y, double *x, double (*w)[O_NUM][W_NUM], double (*w_)[O_NUM][W_NUM], double (*w_store)[O_NUM][W_NUM][O_NUM], double *sum_loss, double *sum_loss_, int type);
void store_weight(double (*w)[O_NUM][W_NUM], double (*w_store)[O_NUM][W_NUM][O_NUM], int type);
void predict_label(log_type *log, double *wx, long count);

#endif /* GRADIENT_H */
