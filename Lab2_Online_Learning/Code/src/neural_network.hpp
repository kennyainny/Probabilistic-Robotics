#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

extern "C"
{
	#include "def_type.h"
}
#include "log.hpp"
void ANN(log_type log);
float sigmoid(float number);
#endif /* NEURAL_NETWORK_H */
