#ifndef BAYSIAN_H
#define BAYSIAN_H

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"
#include "gradient.hpp"

void BLR(log_type train_log, log_type test_log, log_type *BLR_log_online, log_type *BLR_log_stat);
void update_J(double *J, int y, double *x, int k);
void update_P(double (*P)[W_NUM][W_NUM], double *x);
void get_weight(double (*V)[W_NUM][W_NUM], double *m, double (*P)[W_NUM][W_NUM], double *J, double *w);
void predict_output(double *w, double *x, double *y, int k);
void BLR_predict_label(log_type *log, double *y, long count);

#endif /* BAYSIAN_H */
