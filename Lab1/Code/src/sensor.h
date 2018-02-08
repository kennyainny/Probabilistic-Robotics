#ifndef SENSOR_H
#define SENSOR_H

#include "def_type.h"
#include "distribution.h"


<<<<<<< HEAD
float z_ks(map_type map, state_type p_state);
float sensor_model(laser_type laser, state_type state, map_type map, intrinsic_param_type param);
=======
float sensor_model(laser_type laser, state_type state, map_type map, intrinsic_param_type param);
float z_ks(map_type map, state_type p_state);
intrinsic_param_type intrinsic_parameters( state_type p_state,map_type map,sensor_type sensor);
>>>>>>> dd46a1f40e90243b61f0838a09f06ba8698979e4
void new_hornetsoft_sensor(sensor_type *sensor, int size_x, int size_y);
int read_beesoft_sensor(char *sensorName, sensor_type *sensor);

#endif /* SENSOR_H */