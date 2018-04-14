#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP

#include "def_type.hpp"

float Normal_Dist(float x, float m, float sigma);
float Uniform_Dist(float x, float a, float b);
float Narrow_Uniform_Dist(float x, float x_max, float range);
float Map_Calc(float x, float in_min, float in_max, float out_min, float out_max);

#endif /* TRAJECTORY_HPP */