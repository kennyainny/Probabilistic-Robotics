#ifndef NOISE_H
#define NOISE_H

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"
#include "gradient.hpp"

void Add_Noise_1(log_type log, log_type *log_noise);
void Add_Noise_2(log_type log, log_type *log_noise);

#endif /* NOISE_H */
