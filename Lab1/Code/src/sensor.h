#ifndef SENSOR_H
#define SENSOR_H

#include "def_type.h"

void z_ks(map_type map);
void sensor_model();
void new_hornetsoft_sensor(sensor_type *sensor, int size_x, int size_y);
int read_beesoft_sensor(char *sensorName, sensor_type *sensor);

#endif /* SENSOR_H */