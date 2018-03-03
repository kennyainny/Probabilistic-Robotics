#ifndef SENSOR_H
#define SENSOR_H

extern "C"
{
	#include "def_type.h"
}

void new_hornetsoft_log(log_type *log, long size);
int read_log(const char *sensorName, log_type *log);

#endif /* SENSOR_H */
