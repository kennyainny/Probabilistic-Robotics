#include "sensor.hpp"

#define SENSOR_TTIME_STEP 0.01 // sec/step
#define SENSOR_VARIANCE 0.0025*MAX_RANGE // % of maximum reading
#define READING_VARIANCE 5.0

#define SIGMA_INTRINSIC 0.1
#define P_MAX_RANGE 0.01*MAX_RANGE
#define Z_HIT 0.5
#define Z_THETA 0.5
#define Z_MAX 0.0
#define Z_RAND 0.0

using namespace std;
using namespace Eigen;

VectorXd sensor_data((SENSOR_VIEW+1)*3);
Matrix3d R1, R2, R3;
int theta1, theta2, theta3;
double r1, r2, r3;

random_device rd_n;
mt19937 mt_n(rd_n());

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
	float q1 = Normal_Dist((float)theta1, (float)theta1, READING_VARIANCE);
	float q2 = Normal_Dist((float)theta2, (float)theta2, READING_VARIANCE);
	float q3 = Normal_Dist((float)theta3, (float)theta3, READING_VARIANCE);

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

void z_calc(float *z, state_type state, Vector3d p1, Vector3d p2, Vector3d p3){ //three observers
	z[0] = sqrt(pow(state.x - p1(0), 2) + pow(state.y - p1(1), 2));
	z[1] = sqrt(pow(state.x - p2(0), 2) + pow(state.y - p2(1), 2));
	z[2] = sqrt(pow(state.x - p3(0), 2) + pow(state.y - p3(1), 2));
}

void theta_calc(float *theta, state_type state, Vector3d p1, Vector3d p2, Vector3d p3){ //three observers
	theta[0] = atan2(state.y - p1(1), state.x - p1(0));
	if(round(theta[0]) == RAD(0)){
		theta[0] == RAD(0);
	}else{
		if(theta[0] < RAD(0)) theta[0] += RAD(360);
		theta[0] = theta[0] - p1(2) + RAD(SENSOR_VIEW/2);
	}

	theta[1] = atan2(state.y - p2(1), state.x - p2(0));
	if(round(theta[1]) == RAD(0)){
		theta[1] == RAD(0);
	}else{
		if(theta[1] < RAD(0)) theta[1] += RAD(360);
		theta[1] = theta[1] - p2(2) + RAD(SENSOR_VIEW/2);
	}

	theta[2] = atan2(state.y - p3(1), state.x - p3(0));
	if(round(theta[2]) == RAD(0)){
		theta[2] == RAD(0);
	}else{
		if(theta[2] < RAD(0)) theta[2] += RAD(360);
		theta[2] = theta[2] - p3(2) + RAD(SENSOR_VIEW/2);
	}
}

float sensor_model(state_type state, Vector3d p1, Vector3d p2, Vector3d p3){
	int ind;
	float p = 0.0, p_hit = 0.0, p_max = 0.0, p_rand = 0.0, p_theta = 0.0;
	float z[3], theta[3], err, alpha; //three observers
	z_calc(z, state, p1, p2, p3); //calculate z_star
	theta_calc(theta, state, p1, p2, p3);

	for(int i = 0; i < (SENSOR_VIEW+1)*3; i++){	//*3
		p_rand = Uniform_Dist(sensor_data(i), 0, MAX_RANGE);
		p_max = Narrow_Uniform_Dist(sensor_data(i), MAX_RANGE, P_MAX_RANGE);

		if(i < (SENSOR_VIEW+1)){
			if(sensor_data(i) < 0.8*MAX_RANGE && z[0] < MAX_RANGE){			
				p_hit = Normal_Dist(sensor_data(i), z[0], SIGMA_INTRINSIC);
				// p_hit = 0.0;
			}else{
				p_hit = 0.0;
			}

			if(theta[0] >= RAD(0) && theta[0] <= RAD(SENSOR_VIEW) && z[0] < MAX_RANGE){
				ind = (int)DEG(theta[0]);
				err = (sensor_data(ind) - z[0])*(sensor_data(ind) - z[0]);				
				p_theta = Normal_Dist(err, 0, 0.3);
				// p_theta = 0.0;
			}else{
				p_theta = 0.0;
			}
		}
		else if(i < (SENSOR_VIEW+1)*2){
			if(sensor_data(i) < 0.8*MAX_RANGE && z[1] < MAX_RANGE){			
				p_hit = Normal_Dist(sensor_data(i), z[1], SIGMA_INTRINSIC);
			}else{
				p_hit = 0.0;
			}

			if(theta[1] >= RAD(0) && theta[1] <= RAD(SENSOR_VIEW) && z[1] < MAX_RANGE){
				ind = (int)DEG(theta[1]) + (SENSOR_VIEW+1);
				err = (sensor_data(ind) - z[1])*(sensor_data(ind) - z[1]);				
				p_theta = Normal_Dist(err, 0, 0.3);
			}else{
				p_theta = 0.0;
			}
		}else{
			if(sensor_data(i) < 0.8*MAX_RANGE && z[2] < MAX_RANGE){			
				p_hit = Normal_Dist(sensor_data(i), z[2], SIGMA_INTRINSIC);
			}else{
				p_hit = 0.0;
			}

			if(theta[2] >= RAD(0) && theta[2] <= RAD(SENSOR_VIEW) && z[2] < MAX_RANGE){
				ind = (int)DEG(theta[2]) + (SENSOR_VIEW+1)*2;
				err = (sensor_data(ind) - z[2])*(sensor_data(ind) - z[2]);
				p_theta = Normal_Dist(err, 0, 0.3);
			}else{
				p_theta = 0.0;
			}
		}

		// if(z[1] < 2.0 && sensor_data(i) < 0.9*MAX_RANGE)
		// 	cout << sensor_data(i) << ", " << z[1] << ", " << sensor_data(ind) << ", " << ind << ", " << err << ", " << p_theta << ", " << p_hit << ", " << i << endl;
		// if(p_rand != 0) p = p + log(p_rand);		
		// if(p_max != 0) p = p + log(p_max);
		// if(p_hit != 0) p = p + log(p_hit);	
		p = p + Z_RAND*p_rand + Z_MAX*p_max + Z_HIT*p_hit + Z_THETA*p_theta;
	}
	// if(z[1] < 2.0)
	// 	cout << p << endl;
	return p;
}

/* float sensor_model(laser_type laser, state_type state, map_type map){
  float p = 0.0;
  float angle, x_ray_casting, y_ray_casting;
  state_type laser_state; //25 cm offset
  laser_state.theta = state.theta;
  laser_state.x = state.x + (25/map.resolution)*cos(laser_state.theta);
  laser_state.y = state.y + (25/map.resolution)*sin(laser_state.theta);

  if((int)laser_state.x < map.size_x && (int)laser_state.y < map.size_y && 
     (int)laser_state.x > 0 && (int)laser_state.y > 0 &&
      map.prob[(int)laser_state.x][(int)laser_state.y] >= 0 && map.prob[(int)laser_state.x][(int)laser_state.y] > SENSOR_THRESHOLD){

    for (int i = 0; i < 180; i++){
      angle = RAD((float)i) + laser_state.theta;
      x_ray_casting = laser_state.x + (laser.r[i]/map.resolution) * cos(angle - M_PI / 2);
      y_ray_casting = laser_state.y + (laser.r[i]/map.resolution) * sin(angle - M_PI / 2);

      if((int)x_ray_casting < map.size_x && (int)y_ray_casting < map.size_y && 
          (int)x_ray_casting > 0 && (int)y_ray_casting > 0 && 
          map.prob[(int)x_ray_casting][(int)y_ray_casting] > 0 && map.prob[(int)x_ray_casting][(int)y_ray_casting] < SENSOR_THRESHOLD){
          p = p + 1;
      }        
    }    
  }
  return p/180;
} */