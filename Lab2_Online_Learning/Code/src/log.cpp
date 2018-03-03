#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void new_hornetsoft_log(log_type *log, long size)
{
  log->point = (point_type *)calloc(size, sizeof(point_type));
  log->node_id = (long *)calloc(size, sizeof(long));
  log->node_label = (int *)calloc(size, sizeof(int));
  log->feature = (feature_type *)calloc(size, sizeof(feature_type));
}

int read_log(const char *sensorName, log_type *log)
{
  long count = -1 temp_count;
  point_type temp_point;
  
  return 0;
}
