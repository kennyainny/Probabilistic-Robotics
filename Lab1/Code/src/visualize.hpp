#ifndef VISUALIZE_HPP
#define VISUALIZE_HPP

extern "C"
{
	#include "def_type.h"
}
#include <opencv2/opencv.hpp>
using namespace cv;

void MyFilledCircle( Mat img, float x, float y, CvScalar color, float r);
void prob_visualize(map_type map, particle_type *particle, state_type *state, long unsigned int step);

#endif /* VISUALIZE_HPP */