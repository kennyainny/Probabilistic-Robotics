#ifndef SENSOR_H
#define SENSOR_H

#include "def_type.h"
#include "distribution.h"

float z_ks(map_type map, state_type p_state);
float sensor_model(laser_type laser, state_type state, map_type map, intrinsic_param_type param);
void intrinsic_parameters( state_type p_state, map_type map, sensor_type sensor, intrinsic_param_type *param);
void new_hornetsoft_sensor(sensor_type *sensor, int size_x, int size_y);
int read_beesoft_sensor(char *sensorName, sensor_type *sensor);

#endif /* SENSOR_H */