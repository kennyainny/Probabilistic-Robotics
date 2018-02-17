#ifndef VISUALIZE_HPP
#define VISUALIZE_HPP

extern "C"
{
	#include "def_type.h"
}
#include <opencv2/opencv.hpp>
using namespace cv;

void MyFilledCircle( Mat img, float x, float y, CvScalar color, float r);
void particle_visualize(particle_type particle, state_type filtered_state, laser_type laser, map_type map);
void prob_visualize(map_type map, particle_type *particle, state_type *state, unsigned long int step);

#endif /* VISUALIZE_HPP */