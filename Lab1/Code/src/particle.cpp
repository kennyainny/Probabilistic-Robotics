#include "particle.hpp"
#include <iostream>
#include <random>

using namespace std;
random_device rd_p;
mt19937 mt_p(rd_p());

void new_hornetsoft_particle(particle_type *particle, long unsigned int size_p)
{
  particle->state = (state_type *)calloc(size_p, sizeof(state_type));
  particle->prob = (float *)calloc(size_p, sizeof(float));
}

void particle_initialize(map_type map, particle_type *particle){
	// place particles on an unoccupied grid
	printf("Initialze particles\n");

	long unsigned int count_p = 0;
	int space_interval = 10;
	int theta_interval = 45; //every 1 deg is too much
	int theta_number = 360/theta_interval;

	for(int i = 0; i <= map.size_x-1; i = i + space_interval){
		for(int j = 0; j <= map.size_y-1; j = j + space_interval){
			if(map.prob[i][j] > MAP_THRESHOLD){
				count_p = count_p + 1;
			}
		}
	}

	particle->particle_count = count_p*theta_number;
	new_hornetsoft_particle(particle, particle->particle_count);

	count_p = 0;
	for(int i = 0; i < map.size_x; i = i + space_interval){
		for(int j = 0; j < map.size_y; j = j + space_interval){
			if(map.prob[i][j] > MAP_THRESHOLD){
				for(int k = 0; k < 360; k = k + theta_interval){
					particle->prob[count_p] = 1.0/particle->particle_count;
					particle->state[count_p].x = i;
					particle->state[count_p].y = j;
					particle->state[count_p].theta = RAD(float(k));
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
		particle.state[i] = sample_motion_model_odometry(p_odometry, odometry, p_particle.state[i]);
		particle.prob[i] = p_particle.prob[i];			
	}
		
	return particle;
}

particle_type particle_sensor_update(laser_type laser, map_type map, particle_type particle){
	long unsigned int n = particle.particle_count;

	float weight_sum = 0.0;

	for (unsigned int i = 0; i < n; i++){
		particle.prob[i] = sensor_model(laser, particle.state[i], map);
		weight_sum = weight_sum + particle.prob[i];
	}

	// Normalize weights
	for (unsigned int i = 0; i < n; i++){
		particle.prob[i] = particle.prob[i] / weight_sum;
	}
	return particle;
}

particle_type low_variance_sampler(laser_type laser, map_type map, particle_type particle){
	particle_type temp_particle;
	long unsigned int n = particle.particle_count;
	new_hornetsoft_particle(&temp_particle, n);

	uniform_real_distribution<float> distribution(0.0, 1.0);
	float sum_prob = 0.0;

	float weight_sum[n];
	for (unsigned int i = 0; i < n; i++){
		if(i == 0){
			weight_sum[i] = particle.prob[i];
		}else{
			weight_sum[i] = weight_sum[i-1] + particle.prob[i];
		}
	}

	temp_particle.particle_count = n; //for adaptive number of particles

	for(long unsigned int i = 0; i < temp_particle.particle_count; i++){
		float number = distribution(mt_p);

		long unsigned int j = 0;
		if(number < weight_sum[0]){
			j = 0;
		}else if(number > weight_sum[n-1]){
			j = n;
		}else{
			while(number - weight_sum[j] > 0.0){
				j = j + 1;
			}
		}

		temp_particle.state[i] = particle.state[j];
		temp_particle.prob[i] = particle.prob[j];

		sum_prob = sum_prob + temp_particle.prob[i];
	}

	for(long unsigned int i = 0; i < temp_particle.particle_count; i++){
		temp_particle.prob[i] = temp_particle.prob[i] / sum_prob;
		
		particle.state[i] = temp_particle.state[i];
		particle.prob[i] = temp_particle.prob[i];	
	}

	particle.particle_count = temp_particle.particle_count;
	return particle;
}

state_type expected_state(particle_type particle){
	state_type state;
	state.x = 0.0;
	state.y = 0.0;
	state.theta = 0.0;

	for (int i = 0; i < particle.particle_count; i++){
		state.x = state.x + particle.prob[i]*particle.state[i].x;
		state.y = state.y + particle.prob[i]*particle.state[i].y;
		state.theta = state.theta + particle.prob[i]*particle.state[i].theta;
	}

	return state;
}