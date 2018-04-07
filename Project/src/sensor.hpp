#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "def_type.hpp"
#include "distribution.hpp"

using namespace Eigen;

void initialize_sensor(Vector3d p1, Vector3d p2, Vector3d p3);
void Simulate_Sensor(Vector3d p_gt, Vector3d p1, Vector3d p2, Vector3d p3);
void add_dependency();
void add_sensor_noise();

#endif /* TRAJECTORY_HPP */