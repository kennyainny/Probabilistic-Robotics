#include "particle.hpp"

using namespace std;
using namespace Eigen;

random_device rd_p;
mt19937 mt_p(rd_p());

void new_hornetsoft_particle(particle_type *particle, long unsigned int size_p)
{
  particle->state = (state_type *)calloc(size_p, sizeof(state_type));
  particle->prob = (float *)calloc(size_p, sizeof(float));
}

void particle_initialize(particle_type *particle){
	// place particles on unoccupied grids

	Matrix3d R1;
	float theta1, theta2, theta3;

	// decrease these two variables to increase the number of particles
	int radius_interval = 0.25; //place every n metres
	int angle_interval = 10; //place every n deg
	int theta_interval = 45; //heading interval

	int theta_number = 360/theta_interval;
	for(int i = 0; i <= MAX_RANGE; i = i + radius_interval){
		for(int j = 0; j <= SENSOR_VIEW; j = j + angle_interval){
			count_p = count_p + 1; //count the number of grids
		}
	}
	count_p = count_p * 3; //three observers

	particle->particle_count = count_p*theta_number; //the number of particles
	new_hornetsoft_particle(particle, particle->particle_count); //C++ magic

	count_p = 0; //begin assigning particles to each grid
	for(int n = 0; n < 3; n++){
		for(int i = 0; i <= MAX_RANGE; i = i + radius_interval){
			for(int j = 0; j <= SENSOR_VIEW; j = j + angle_interval){
				theta1 = p1(2) - RAD(SENSOR_VIEW/2) + RAD(j);
				theta2 = p2(2) - RAD(SENSOR_VIEW/2) + RAD(j);
				theta3 = p3(2) - RAD(SENSOR_VIEW/2) + RAD(j);

				for(int k = 0; k < 360; k = k + theta_interval){
					particle->prob[count_p] = 1.0/particle->particle_count; //uniformly distributed
					if(n == 1){ //first observer space
						particle->state[count_p].x = p1(0) + i*cos(theta1); //place at x location
						particle->state[count_p].y = p1(1) + j*cos(theta1); //place at y location
						particle->state[count_p].theta = RAD(float(k)); //place at different angle
					}else if(n == 2){ //second observer space
						particle->state[count_p].x = p2(0) + i*cos(theta2); //place at x location
						particle->state[count_p].y = p2(1) + j*cos(theta2); //place at y location
						particle->state[count_p].theta = RAD(float(k)); //place at different angle
					}else{ //third observer space
						particle->state[count_p].x = p3(0) + i*cos(theta3); //place at x location
						particle->state[count_p].y = p3(1) + j*cos(theta3); //place at y location
						particle->state[count_p].theta = RAD(float(k)); //place at different angle
					}				
					count_p = count_p + 1;
				}
			}
		}
	}
}

particle_type particle_motion_update(state_type p_odometry, state_type odometry, particle_type p_particle){
	
	particle_type particle;
	long unsigned int n = p_particle.particle_count;
	new_hornetsoft_particle(&particle, n);
	particle.particle_count = n;

	for (unsigned int i = 0; i < p_particle.particle_count; i++){
		particle.state[i] = sample_motion_model_odometry(p_odometry, odometry, p_particle.state[i]); //getting a new particle's location
		particle.prob[i] = p_particle.prob[i]; // still having the same probability			
	}
		
	return particle;
}

particle_type particle_sensor_update(laser_type laser, particle_type particle){
	long unsigned int n = particle.particle_count;

	float weight_sum = 0.0; //normalizer

	for (unsigned int i = 0; i < n; i++){
		particle.prob[i] = sensor_model(laser, particle.state[i]); //update each particle's probability based on sensor reading
		weight_sum = weight_sum + particle.prob[i]; //accumulate
	}

	for (unsigned int i = 0; i < n; i++){
		particle.prob[i] = particle.prob[i] / weight_sum; //normailizing
		if(isnan(particle.prob[i])){
			particle.prob[i] = 0.0;
		}
	}
	return particle;
}

particle_type low_variance_sampler(laser_type laser, particle_type particle){
	particle_type temp_particle;
	long unsigned int n = particle.particle_count;
	new_hornetsoft_particle(&temp_particle, n);

	uniform_real_distribution<float> distribution(0.0, 1.0); //uniform distribution
	float sum_prob = 0.0; //normalizer

	float weight_cdf[n]; //long array from zero to one
	for (unsigned int i = 0; i < n; i++){
		if(i == 0){
			weight_cdf[i] = particle.prob[i];
		}else{
			weight_cdf[i] = weight_cdf[i-1] + particle.prob[i];
		}
	}

	//For adaptive number of particles, n must be a result of some function.
	//e.g. changing n based on variance
	temp_particle.particle_count = n; 

	for(long unsigned int i = 0; i < temp_particle.particle_count; i++){
		float number = distribution(mt_p); //uniformly pick up a random number

		long unsigned int j = 0;
		if(number < weight_cdf[0]){ //If the number is smaller than the prob of the first element
			j = 0; //then it becomes that particle
		}else if(number > weight_cdf[n-1]){ //If the number is greater than the prob of the last element
			j = n; //then it becomes that particle
		}else{
			while(number - weight_cdf[j] > 0.0){ //fall in between
				j = j + 1;
			}
		}

		temp_particle.state[i] = particle.state[j]; //temporary save the location of the new set of particles
		temp_particle.prob[i] = particle.prob[j]; //temporary save the prob of the new set of particles

		sum_prob = sum_prob + temp_particle.prob[i]; //accumulate
	}

	for(long unsigned int i = 0; i < temp_particle.particle_count; i++){
		temp_particle.prob[i] = temp_particle.prob[i] / sum_prob; //normailizing
		
		particle.state[i] = temp_particle.state[i]; //replacing the location
		particle.prob[i] = temp_particle.prob[i]; //replacing the probability
	}

	particle.particle_count = temp_particle.particle_count; ////replacing the number of particles
	return particle;
}

state_type expected_state(particle_type particle){
	state_type state;
	state.x = 0.0;
	state.y = 0.0;
	state.theta = 0.0;

	for (int i = 0; i < particle.particle_count; i++){ //since the probability is already normalized
		state.x = state.x + particle.prob[i]*particle.state[i].x; //expected x
		state.y = state.y + particle.prob[i]*particle.state[i].y; //expected y
		state.theta = state.theta + particle.prob[i]*particle.state[i].theta; //expected theta
	}

	return state;
}