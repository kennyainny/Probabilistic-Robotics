#ifndef GP_RBF_H
#define GP_RBF_H

#define O_NUM 2
#define W_NUM 9

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"

double K_RBF(feature_type x1, feature_type x2, double l, double sigma);
void GP_RBF(log_type train_log, log_type test_log, log_type *GP_RBF_Train, log_type *GP_RBF_Test);

#endif /* GP_RBF_H */
