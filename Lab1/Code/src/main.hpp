#ifndef MAIN_HPP
#define MAIN_HPP

typedef struct {
  float x, y, theta;
} state_type;

extern "C"
{
	#include "map.h"
	#include "sensor.h"
}

#include "visualize.hpp"
#include "motion.hpp"

#endif /* MAIN_HPP */