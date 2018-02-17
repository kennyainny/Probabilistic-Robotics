#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "MonteCarloLocalization.h"

#ifdef VIZ
	#include <opencv/cv.h>
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
	#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#endif

using namespace std;



MonteCarloLocalization::MonteCarloLocalization(){
	// _num_particles = 100;
	// _particles = new part[_num_particles];
	// // _alpha[0] = 0.01;_alpha[1] = 0.01;
	// // _alpha[2] = 0.1;_alpha[3] = 0.1;

	// _alpha[0] = 0.005;_alpha[1] = 0.005;
	// _alpha[2] = 0.01;_alpha[3] = 0.01;

	// _threshold = 0.7;
	// _zhit = 0.8;
	// _znoise = 0.1;
	// _zshort = 0.095;
	// _zmax = 0.005;
	// _hit_sigma = 20;
	// _lamda_short = 0.0005;
	// _max_laser_range = 800.0;
	// _min_step = 1;

	// srand(time(NULL));
}

MonteCarloLocalization::~MonteCarloLocalization(){
	// delete[] _particles;
	// _map_image = Mat();
}

void MonteCarloLocalization::init_map(map_type map){
	_map = map;

	_map_image = Mat::zeros( _map.size_x, map.size_y, CV_32FC1 );

	cout << "Image Property" << endl;
	cout << "Row: " << _map_image.rows << " Col: " << _map_image.cols << endl;
	cout << "Step: " << _map_image.step << " Dim: " << _map_image.dims << endl;
	cout << "ElemSize: " << _map_image.elemSize() << " Depth: " << _map_image.depth() << endl;
	cout << "Channels: " << _map_image.channels() << endl;

	// unsigned char *imgMat = (unsigned char*)(image.data);
	float max_value = -10, min_value = 10;
	for (unsigned int i = 0; i < _map_image.rows; i++)
		for (unsigned int j = 0; j < _map_image.cols; j++){
			if (_map.prob[i][j] >= 0.0 && _map.prob[i][j] <= 1.0)
				_map_image.at<float>(i, j) = _map.prob[i][j];
			else
				_map_image.at<float>(i, j) = 0;
			max_value = max_value >  _map.prob[i][j] ? max_value : _map.prob[i][j];
			min_value = min_value <  _map.prob[i][j] ? min_value : _map.prob[i][j];
		}

		cout << "Max Value " << max_value << " Min Value " << min_value << endl;
		imshow("Image", _map_image);
		waitKey( 3000 );


}

void MonteCarloLocalization::_visualize_particles(particle_type particle){
  	// Draw circles
	Mat new_image;
	// _map_image.clone();
	cvtColor(_map_image, new_image, CV_GRAY2RGB);
	for (unsigned long int i = 0; i < particle.particle_count; i++){
  		// circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
		// circle(new_image, Point(_particles[i].y, _particles[i].x), 2, Scalar(0, 0, 255), 2, 8);
		circle(new_image, Point(particle.state[i].y, particle.state[i].x), 2, Scalar(0, 0, 255), 2, 8);
	}
  	// circle( image, Point( 200, 200 ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
  	// printf("show image\n");
	imshow("Image", new_image);
	waitKey( 10 );
	new_image = Mat();
}

void MonteCarloLocalization::_visualize_particle_with_log(state_type particle_state, laser_type laser, map_type map){
	// Draw circles
	Mat new_image;
	Mat tmp_img = _map_image.clone();
	cvtColor(_map_image, new_image, CV_GRAY2RGB);

	float x = particle_state.x + 2.5 * cos(particle_state.theta);
	float y = particle_state.y + 2.5 * sin(particle_state.theta);
	circle(new_image, Point(y, x), 3, Scalar(0, 0, 255), 2, 8);

	for (unsigned int i = 0; i < RANGE_LEN; i++){
		float angle = (float)i * PI / 180 + particle_state.theta;
		for (unsigned int d = 0; d < (int)(laser.r[i]/10); d++){
			int x_end = (int)(d * cos(angle - PI / 2) + x);
			int y_end = (int)(d * sin(angle - PI / 2) + y);
			if (x_end < map.min_x || x_end > map.max_x || 
				y_end < map.min_y || y_end > map.max_y)
				continue;
			circle(new_image, Point(y_end, x_end), 1, Scalar(255, 0, 255), 2, 8);
		}
	}

	imshow("Estimation", new_image);
	waitKey( 10 );
}