#ifndef GRADIENT_H
#define GRADIENT_H

extern "C"
{
	#include "def_type.h"
}

log_type Gradient_Descent(log_type train_log, log_type test_log);

#endif /* GRADIENT_H */
