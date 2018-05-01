#ifndef REGRET_H
#define REGRET_H

#define ALPHA 0.0001
#define REGRET_NUM 100
#define ERR 0.1

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"
#include "GP_RBF.hpp"

double min_loss(log_type log);

#endif /* REGRET_H */
