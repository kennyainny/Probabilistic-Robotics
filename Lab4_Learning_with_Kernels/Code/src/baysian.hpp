#ifndef BAYSIAN_H
#define BAYSIAN_H

#define O_NUM 2
#define W_NUM 9

extern "C"
{
	#include "def_type.h"
}

#include "log.hpp"

// double CostFunction(double omege, double X, double Y);


// double K_RBF(feature_type x1, feature_type x2, double l, double sigma);
void KLR_RBF(log_type train_log, log_type test_log, log_type *GP_RBF_Train, log_type *GP_RBF_Test);

// double BLR(log_type train_log, log_type test_log, log_type *BLR_log_online, log_type *BLR_log_stat);
// void update_J(double *J, int y, double *x, int k);
// void update_P(double (*P)[W_NUM][W_NUM], double *x);
// void get_weight(double (*V)[W_NUM][W_NUM], double *m, double (*P)[W_NUM][W_NUM], double *J, double *w);
// void predict_output(double *w, double *x, double *y, int k);
// void BLR_predict_label(log_type *log, double *y, long count);
// double calc_loss(int *y, double *y_);

#endif /* BAYSIAN_H */

