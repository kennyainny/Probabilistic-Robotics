#include <iostream>
#define VIZ

#ifdef VIZ
	#include <opencv2/core/core.hpp>
	#include <opencv2/highgui/highgui.hpp>
	using namespace cv;
#endif

using namespace std;

#define PI 3.1415926
#define RANGE_LEN 180

extern "C"
{
	#include "def_type.h"
}

class MonteCarloLocalization{
public:
	MonteCarloLocalization();
	~MonteCarloLocalization();

	// Initialize
	void init_map(map_type map);
	Mat _map_image;
	void _visualize_particles(particle_type particle);
	void _visualize_particle_with_log(state_type particle_state, laser_type laser, map_type map);

protected:

	map_type _map;
	float _max_laser_range;
	float _min_step;
};