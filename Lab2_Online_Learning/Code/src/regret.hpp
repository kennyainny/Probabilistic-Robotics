#ifndef REGRET_H
#define REGRET_H

#define ALPHA 0.0001
#define LOOP_NUM 10
#define ERR 0.1

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"
#include "gradient.hpp"

void min_loss(log_type log, double *sum_loss);

#endif /* REGRET_H */
