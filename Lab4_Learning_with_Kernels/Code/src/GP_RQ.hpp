#ifndef GP_RQ_H //rational quadratic covariance function RW page 87
#define GP_RQ_H

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"

double K_RQ(feature_type x1, feature_type x2, double l, double sigma);
void GP_RQ(log_type train_log, log_type test_log, log_type *GP_RQ_Train, log_type *GP_RQ_Test);

#endif /* GP_RQ_H */
