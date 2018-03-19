#include "visualize.hpp"

using namespace Eigen;
using namespace std;
using namespace matplotlibcpp;

vector<double> x_0(2), y_0(2), x_(3), y_(3);
vector<double> x_1(90+1+2), y_1(90+1+2), x_2(90+1+2), y_2(90+1+2), x_3(90+1+2), y_3(90+1+2);
Vector3d v_sensor(MAX_RANGE, 0, OMEGA);

void sensor_view(Vector3d v, vector<double> *x, vector<double> *y){
	// Vector3d v_temp;
	// Matrix3d R1, R;
	// R1 = AngleAxisd(RAD(1), Vector3d::UnitZ());
	// R = AngleAxisd(v(2)-RAD(45), Vector3d::UnitZ());

	// *x->at(0) = v(0);
	// *y->at(0) = v(1);

	// v_temp = R*v;
	// for(int i = 1; i < 92; i++){
	// 	*x->at(i) = v_temp(0) + *x->at(0);
	// 	*y->at(i) = v_temp(1) + *y->at(0);
	// 	v = v_temp;
	// 	v_temp = R1*v;
	// }
}

void initialize_visualization(Vector3d v1, Vector3d v2, Vector3d v3){
	Vector3d v_temp, v;
	Matrix3d R, R1;
	R1 = AngleAxisd(RAD(1), Vector3d::UnitZ());

	R = AngleAxisd(v1(2)-RAD(45), Vector3d::UnitZ());
	x_.at(0) = v1(0);
	y_.at(0) = v1(1);
	x_1.at(0) = v1(0);
	y_1.at(0) = v1(1);
	x_1.at(92) = v1(0);
	y_1.at(92) = v1(1);

	v_temp = R*v_sensor;
	for(int i = 1; i < 92; i++){
		x_1.at(i) = v_temp(0) + v1(0);
		y_1.at(i) = v_temp(1) + v1(1);
		v = v_temp;
		v_temp = R1*v;
	}

	R = AngleAxisd(v2(2)-RAD(45), Vector3d::UnitZ());
	x_.at(1) = v2(0);
	y_.at(1) = v2(1);
	x_2.at(0) = v2(0);
	y_2.at(0) = v2(1);
	x_2.at(92) = v2(0);
	y_2.at(92) = v2(1);

	v_temp = R*v_sensor;
	for(int i = 1; i < 92; i++){
		x_2.at(i) = v_temp(0) + v2(0);
		y_2.at(i) = v_temp(1) + v2(1);
		v = v_temp;
		v_temp = R1*v;
	}

	R = AngleAxisd(v3(2)-RAD(45), Vector3d::UnitZ());
	x_.at(2) = v3(0);
	y_.at(2) = v3(1);
	x_3.at(0) = v3(0);
	y_3.at(0) = v3(1);
	x_3.at(92) = v3(0);
	y_3.at(92) = v3(1);

	v_temp = R*v_sensor;
	for(int i = 1; i < 92; i++){
		x_3.at(i) = v_temp(0) + v3(0);
		y_3.at(i) = v_temp(1) + v3(1);
		v = v_temp;
		v_temp = R1*v;
	}
}

void visualization(Vector3d v0){
	x_0.at(0) = 0.0;
	y_0.at(0) = 0.0;
	x_0.at(1) = v0(0);
	y_0.at(1) = v0(1);

	plot(x_0, y_0, "r.", x_1, y_1, "b-", x_2, y_2, "b-", x_3, y_3, "b-", x_, y_, "b.");
	axis("equal");
	xlim(-3, 3);
	ylim(-3, 3);	
	pause(0.001);
	clf();
}