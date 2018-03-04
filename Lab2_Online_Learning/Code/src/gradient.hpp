#ifndef GRADIENT_H
#define GRADIENT_H

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"

void Gradient_Descent(log_type train_log, log_type test_log, log_type *gradient_log);

#endif /* GRADIENT_H */
