#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include "def_type.h"

float normal_dist(float x, float x_max, float m, float var); // var = std^2
float exp_dist(float x, float x_max, float lambda);
float narrow_uniform_dist(float x, float x_max, float range);
float uniform_dist(float x, float x_max);

#endif /* DISTRIBUTION_H */