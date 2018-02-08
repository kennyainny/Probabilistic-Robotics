#ifndef SENSOR_H
#define SENSOR_H

#include "def_type.h"
#include "distribution.h"

<<<<<<< HEAD
void z_ks(map_type map, state_type p_state);
float sensor_model(laser_type laser, state_type state, map_type map, intrinsic_param_type param);
=======
float z_ks(map_type map, state_type p_state);
<<<<<<< HEAD
float intrinsic_parameters(map_type map, state_type p_state);
=======
>>>>>>> 119773e64978b25beeb279f586b42e5e36273cae
>>>>>>> 7a7f86aee842cc95fd68d061d5cead550cbd33a8
void new_hornetsoft_sensor(sensor_type *sensor, int size_x, int size_y);
int read_beesoft_sensor(char *sensorName, sensor_type *sensor);

#endif /* SENSOR_H */