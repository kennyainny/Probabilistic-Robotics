#ifndef LOG_H
#define LOG_H

extern "C"
{
	#include "def_type.h"
}

void new_hornetsoft_log(log_type *log);
int read_log(const char *logName, log_type *log);
void sort_log(log_type log, log_type *log_sort);
void pick_two_classes(log_type log, log_type *log_two);

#endif /* LOG_H */
