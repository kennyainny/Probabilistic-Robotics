#ifndef GRADIENT_H
#define GRADIENT_H

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"

log_type Gradient_Descent(log_type train_log, log_type test_log);

#endif /* GRADIENT_H */
