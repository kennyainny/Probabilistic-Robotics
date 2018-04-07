#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP

#include "def_type.hpp"
#include <math.h>

float Normal_Dist(float x, float m, float sigma);
float Uniform_Dist(float x, float a, float b);
float Narrow_Uniform_Dist(float x, float x_max, float range);

#endif /* TRAJECTORY_HPP */