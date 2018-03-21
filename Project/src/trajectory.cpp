#include "trajectory.hpp"

#define TIME_STEP 0.01 // sec/step

using namespace std;
using namespace Eigen;

Vector3d Simulate_Trajectory(Vector3d p_gt_old){
	Vector3d p(0, 0, OMEGA); //x , y, Omega

	Matrix3d R;
	R = AngleAxisd(OMEGA*TIME_STEP, Vector3d::UnitZ());
	// R = AngleAxisd(RAD(90), Vector3d::UnitZ());
	p = R*p_gt_old;

	return p;
}
