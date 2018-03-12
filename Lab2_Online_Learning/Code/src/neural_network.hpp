#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"
#include "gradient.hpp"
void ANN(log_type train_log, log_type test_log, log_type *NN_log_online, log_type *NN_log_stat);
void ANN_ONLINE(log_type train_log, log_type test_log, log_type *NN_log_online, log_type *NN_log_stat);
void assign_output(log_type log, long count, int *y);
float sigmoid(float number);

#endif /* NEURAL_NETWORK_H */
