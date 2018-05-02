#include "visualize.hpp"

using namespace Eigen;
using namespace std;
using namespace matplotlibcpp;

vector<double> x_particle, y_particle, theta_particle, x_state(1), y_state(1), theta_state(1);
vector<double> x_0(1), y_0(1), x_(3), y_(3), x_sensor((SENSOR_VIEW+1)*3), y_sensor((SENSOR_VIEW+1)*3), x_sensor_avg(1), y_sensor_avg(1);
vector<double> x_1(SENSOR_VIEW+1+2), y_1(SENSOR_VIEW+1+2), x_2(SENSOR_VIEW+1+2), y_2(SENSOR_VIEW+1+2), x_3(SENSOR_VIEW+1+2), y_3(SENSOR_VIEW+1+2);
Vector3d p_sensor(MAX_RANGE, 0, OMEGA);
Matrix3d R_sensor1, R_sensor2, R_sensor3;
extern VectorXd sensor_data;

void initialize_visualization(Vector3d p1, Vector3d p2, Vector3d p3){
	Vector3d p_temp, p;
	Matrix3d R, R1;
	R1 = AngleAxisd(RAD(1), Vector3d::UnitZ()); //R for 1 deg rotation

	R = AngleAxisd(p1(2)-RAD(SENSOR_VIEW/2), Vector3d::UnitZ());
	x_.at(0) = p1(0);
	y_.at(0) = p1(1);
	x_1.at(0) = p1(0);
	y_1.at(0) = p1(1);
	x_1.at(SENSOR_VIEW+2) = p1(0);
	y_1.at(SENSOR_VIEW+2) = p1(1);

	p_temp = R*p_sensor;
	for(int i = 1; i < SENSOR_VIEW+2; i++){
		x_1.at(i) = p_temp(0) + p1(0);
		y_1.at(i) = p_temp(1) + p1(1);
		p = p_temp;
		p_temp = R1*p;
	}

	R = AngleAxisd(p2(2)-RAD(SENSOR_VIEW/2), Vector3d::UnitZ());
	x_.at(1) = p2(0);
	y_.at(1) = p2(1);
	x_2.at(0) = p2(0);
	y_2.at(0) = p2(1);
	x_2.at(SENSOR_VIEW+2) = p2(0);
	y_2.at(SENSOR_VIEW+2) = p2(1);

	p_temp = R*p_sensor;
	for(int i = 1; i < SENSOR_VIEW+2; i++){
		x_2.at(i) = p_temp(0) + p2(0);
		y_2.at(i) = p_temp(1) + p2(1);
		p = p_temp;
		p_temp = R1*p;
	}

	R = AngleAxisd(p3(2)-RAD(SENSOR_VIEW/2), Vector3d::UnitZ());
	x_.at(2) = p3(0);
	y_.at(2) = p3(1);
	x_3.at(0) = p3(0);
	y_3.at(0) = p3(1);
	x_3.at(SENSOR_VIEW+2) = p3(0);
	y_3.at(SENSOR_VIEW+2) = p3(1);

	p_temp = R*p_sensor;
	for(int i = 1; i < SENSOR_VIEW+2; i++){
		x_3.at(i) = p_temp(0) + p3(0);
		y_3.at(i) = p_temp(1) + p3(1);
		p = p_temp;
		p_temp = R1*p;
	}

	R_sensor1 = AngleAxisd(p1(2) - RAD(45), Vector3d::UnitZ());
	R_sensor2 = AngleAxisd(p2(2) - RAD(45), Vector3d::UnitZ());
	R_sensor3 = AngleAxisd(p3(2) - RAD(45), Vector3d::UnitZ());
}

void plot_ground_truth(Vector3d v_gt){
	x_0.at(0) = v_gt(0);
	y_0.at(0) = v_gt(1);
}

void plot_sensor(Vector3d p1, Vector3d p2, Vector3d p3){
	Vector3d p_temp, p;
	double theta;
	int j1 = 0, j2 = 0, j3 = 0;
	float r_sensor_avg = 0, theta_sensor_avg = 0;
	float r_sensor_avg_temp = 0, theta_sensor_avg_temp = 0;
	float r_sensor_avg1 = 0, theta_sensor_avg1 = 0;
	float r_sensor_avg2 = 0, theta_sensor_avg2 = 0;
	float r_sensor_avg3 = 0, theta_sensor_avg3 = 0;

	for(int i = 0; i < (SENSOR_VIEW+1)*3; i++){
		if(i < (SENSOR_VIEW+1)){
			p_temp(0) = sensor_data(i) * cos(RAD(i));
			p_temp(1) = sensor_data(i) * sin(RAD(i));
			p = R_sensor1*p_temp + p1;

			if(sensor_data[i] < MAX_RANGE){
				r_sensor_avg1 = r_sensor_avg1 + sensor_data[i];
				theta_sensor_avg1 = theta_sensor_avg1 + i;
				j1 = j1 + 1;
			}
		}else if(i < (SENSOR_VIEW+1)*2){
			theta = i - (SENSOR_VIEW+1);
			p_temp(0) = sensor_data(i) * cos(RAD(theta));
			p_temp(1) = sensor_data(i) * sin(RAD(theta));
			p = R_sensor2*p_temp + p2;

			if(sensor_data[i] < MAX_RANGE){
				r_sensor_avg2 = r_sensor_avg2 + sensor_data[i];
				theta_sensor_avg2 = theta_sensor_avg2 + theta;
				j2 = j2 + 1;
			}
		}else{
			theta = i - (SENSOR_VIEW+1)*2;
			p_temp(0) = sensor_data(i) * cos(RAD(theta));
			p_temp(1) = sensor_data(i) * sin(RAD(theta));
			p = R_sensor3*p_temp + p3;

			if(sensor_data[i] < MAX_RANGE){
				r_sensor_avg3 = r_sensor_avg3 + sensor_data[i];
				theta_sensor_avg3 = theta_sensor_avg3 + theta;
				j3 = j3 + 1;
			}
		}
		x_sensor.at(i) = p(0);
		y_sensor.at(i) = p(1);
	}

	r_sensor_avg_temp = r_sensor_avg1;
	r_sensor_avg = r_sensor_avg_temp / j1;
	theta_sensor_avg = theta_sensor_avg1 / j1;
	p_temp(0) = r_sensor_avg * cos(RAD(theta_sensor_avg));
	p_temp(1) = r_sensor_avg * sin(RAD(theta_sensor_avg));
	p = R_sensor1*p_temp + p1;

	if(r_sensor_avg2 > r_sensor_avg_temp){
		r_sensor_avg_temp = r_sensor_avg2;
		r_sensor_avg = r_sensor_avg_temp / j2;
		theta_sensor_avg = theta_sensor_avg2 / j2;
		p_temp(0) = r_sensor_avg * cos(RAD(theta_sensor_avg));
		p_temp(1) = r_sensor_avg * sin(RAD(theta_sensor_avg));
		p = R_sensor2*p_temp + p2;
	}

	if(r_sensor_avg3 > r_sensor_avg_temp){
		r_sensor_avg_temp = r_sensor_avg3;
		r_sensor_avg = r_sensor_avg_temp / j3;
		theta_sensor_avg = theta_sensor_avg3 / j3;
		p_temp(0) = r_sensor_avg * cos(RAD(theta_sensor_avg));
		p_temp(1) = r_sensor_avg * sin(RAD(theta_sensor_avg));
		p = R_sensor3*p_temp + p3;
	}

	x_sensor_avg.at(0) = p(0);
	y_sensor_avg.at(0) = p(1);
}

void plot_particle(particle_type particle){
	for(int i = 0; i < particle.particle_count; i++){
		x_particle.resize(i+1, particle.state[i].x);
		y_particle.resize(i+1, particle.state[i].y);
		theta_particle.resize(i+1, particle.state[i].theta);
	}
}

void visualization(Vector3d p_gt, Vector3d p1, Vector3d p2, Vector3d p3, particle_type particle, state_type state){
	plot_ground_truth(p_gt);
	plot_sensor(p1, p2, p3);
	plot_particle(particle);

	x_state.at(0) = (double)state.x;
	y_state.at(0) = (double)state.y;

	float sensor_avg_err = sqrt((x_sensor_avg.at(0) - x_0.at(0))*(x_sensor_avg.at(0) - x_0.at(0)) + ((y_sensor_avg.at(0) - y_0.at(0))*(y_sensor_avg.at(0) - y_0.at(0))));
	float particle_avg_err = sqrt((x_state.at(0) - x_0.at(0))*(x_state.at(0) - x_0.at(0)) + ((y_state.at(0) - y_0.at(0))*(y_state.at(0) - y_0.at(0))));
	cout << sensor_avg_err << ", " << particle_avg_err << endl;
	// plot(x_1, y_1, "b-", x_2, y_2, "b-", x_3, y_3, "b-", x_, y_, "b.", x_sensor, y_sensor, "b.");
	// plot(x_1, y_1, "b-", x_2, y_2, "b-", x_3, y_3, "b-", x_, y_, "b.", x_sensor, y_sensor, "b.", x_0, y_0, "r.");
	plot(x_1, y_1, "b-", x_2, y_2, "b-", x_3, y_3, "b-", x_, y_, "b.", x_particle, y_particle, "y.", x_sensor, y_sensor, "b.", x_state, y_state, "k.", x_sensor_avg, y_sensor_avg, "c.", x_0, y_0, "r.");
	axis("equal");
	xlim(-3, 3);
	ylim(-3, 3);
	// show();
	pause(0.001);
	clf();
}