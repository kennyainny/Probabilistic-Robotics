#ifndef SENSOR_H
#define SENSOR_H

#include "def_type.h"

float z_ks(map_type map, state_type p_state);
float intrinsic_parameters(map_type map, state_type p_state);
void new_hornetsoft_sensor(sensor_type *sensor, int size_x, int size_y);
int read_beesoft_sensor(char *sensorName, sensor_type *sensor);

#endif /* SENSOR_H */