#include "sensor.hpp"

#define SENSOR_TTIME_STEP 0.01 // sec/step
#define SENSOR_VARIANCE 0.003*MAX_RANGE // % of maximum reading
#define READING_VARIANCE 5.0

#define SIGMA_INTRINSIC 1.0
#define Z_HIT 0.7
#define Z_MAX 0.2
#define Z_RAND 0.1

VectorXd sensor_data((SENSOR_VIEW+1)*3);
Matrix3d R1, R2, R3;
int theta1, theta2, theta3;
double r1, r2, r3;

random_device rd_n;
mt19937 mt_n(rd_n());

using namespace std;
using namespace Eigen;

void initialize_sensor(Vector3d p1, Vector3d p2, Vector3d p3){
	R1 = AngleAxisd(-(p1(2) - RAD(45)), Vector3d::UnitZ());
	R2 = AngleAxisd(-(p2(2) - RAD(45)), Vector3d::UnitZ());
	R3 = AngleAxisd(-(p3(2) - RAD(45)), Vector3d::UnitZ());
}

void Simulate_Sensor(Vector3d p_gt, Vector3d p1, Vector3d p2, Vector3d p3){
	Vector3d dp1, dp2, dp3; //x , y, Omega

	dp1 = R1*(p_gt - p1);
	theta1 = (int)floor(DEG(atan2(dp1(1), dp1(0))));
	r1 = sqrt(dp1(0)*dp1(0) + dp1(1)*dp1(1));

	dp2 = R2*(p_gt - p2);
	theta2 = (int)floor(DEG(atan2(dp2(1), dp2(0))));
	r2 = sqrt(dp2(0)*dp2(0) + dp2(1)*dp2(1));

	dp3 = R3*(p_gt - p3);
	theta3 = (int)floor(DEG(atan2(dp3(1), dp3(0))));
	r3 = sqrt(dp3(0)*dp3(0) + dp3(1)*dp3(1));

	for(int i = 0; i < (SENSOR_VIEW+1)*3; i++){
		if(i < (SENSOR_VIEW+1)){
			if(i == theta1 && r1 < MAX_RANGE){
				sensor_data[i] = r1;
			}else{
				sensor_data[i] = MAX_RANGE;
			}
		}else if(i < (SENSOR_VIEW+1)*2){
			if(i == (theta2 + (SENSOR_VIEW+1)) && r2 < MAX_RANGE){
				sensor_data[i] = r2;
			}else{
				sensor_data[i] = MAX_RANGE;
			}
		}else{
			if(i == (theta3 + (SENSOR_VIEW+1)*2) && r3 < MAX_RANGE){
				sensor_data[i] = r3;
			}else{
				sensor_data[i] = MAX_RANGE;
			}
		}
	}
}

void add_dependency(){
	float q1 = Normal_Distribution((float)theta1, (float)theta1, READING_VARIANCE);
	float q2 = Normal_Distribution((float)theta2, (float)theta2, READING_VARIANCE);
	float q3 = Normal_Distribution((float)theta3, (float)theta3, READING_VARIANCE);

	for(int i = 0; i < (SENSOR_VIEW+1)*3; i++){
		if(i < (SENSOR_VIEW+1)){
			if(i != theta1 && r1 < MAX_RANGE){
				float p = Normal_Dist((float)i, (float)theta1, READING_VARIANCE)/q1;
				sensor_data[i] -= p*(MAX_RANGE - r1);
			}
		}else if(i < (SENSOR_VIEW+1)*2){
			if(i != (theta2 + (SENSOR_VIEW+1)) && r2 < MAX_RANGE){
				float p = Normal_Dist((float)(i - (SENSOR_VIEW+1)), (float)theta2, READING_VARIANCE)/q2;
				sensor_data[i] -= p*(MAX_RANGE - r2);
			}
		}else{
			if(i != (theta3 + (SENSOR_VIEW+1)*2) && r3 < MAX_RANGE){
				float p = Normal_Dist((float)(i - (SENSOR_VIEW+1)*2), (float)theta3, READING_VARIANCE)/q3;
				sensor_data[i] -= p*(MAX_RANGE - r3);
			}
		}
	}
}

void add_sensor_noise(){
	normal_distribution<float> distr;
	distr.param(normal_distribution<float>::param_type(0.0, sqrt(SENSOR_VARIANCE)));

	for(int i = 0; i < (SENSOR_VIEW+1)*3; i++){
		sensor_data[i] = sensor_data[i] + distr(mt_n);
		if(sensor_data[i] > MAX_RANGE){
			sensor_data[i] = MAX_RANGE;
		}
	}
}

void sensor_model(){

}