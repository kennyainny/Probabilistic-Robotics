#include "particle.hpp"
#include <iostream>
#include <random>

using namespace std;
random_device rdm;
mt19937 mtd(rdm());

void new_hornetsoft_particle(particle_type *particle, long unsigned int size_p)
{
  particle->state = (state_type *)calloc(size_p, sizeof(state_type));
  particle->prob = (float *)calloc(size_p, sizeof(float));
}

void particle_initialize(map_type map, particle_type *particle){
	// place particles on a grid that isn't fully occupied

	printf("Initialze particles\n");

	long unsigned int count_p = 0;
	int space_interval = 30;
	int theta_interval = 90; //every 1 deg is too much
	int theta_number = 360/theta_interval;

	for(int i = 0; i <= map.size_x-1; i = i + space_interval){
		for(int j = 0; j <= map.size_y-1; j = j + space_interval){
			// if(map.prob[i][j] > -1.0 && map.prob[i][j] < 1.0){
			if(map.prob[i][j] == 0.0){
				count_p = count_p + 1;
			}
		}
	}

	particle->particle_count = count_p*theta_number;
	new_hornetsoft_particle(particle, particle->particle_count);

	printf("%lu\n", count_p);	
	printf("%lu\n", particle->particle_count);	

	count_p = 0;
	for(int i = 0; i < map.size_x; i = i + space_interval){
		for(int j = 0; j < map.size_y; j = j + space_interval){
			// if(map.prob[i][j] > -1.0 && map.prob[i][j] < 1.0){
			if(map.prob[i][j] == 0.0){
				for(int k = 0; k < 360; k = k + theta_interval){
					particle->prob[count_p] = 1.0/particle->particle_count;
					particle->state[count_p].x = i;
					particle->state[count_p].y = j;
					particle->state[count_p].theta = RAD(k);
					if(particle->state[count_p].theta > M_PI){
						particle->state[count_p].theta = particle->state[count_p].theta - 2*M_PI;
					}
					count_p = count_p + 1;
				}
			}
		}
	}
	// printf("%e\n", particle->prob[1]);
}

particle_type particle_filter(particle_type p_particle, laser_type laser, odometry_type p_odometry, odometry_type odometry, map_type map, intrinsic_param_type param)
{	//each particle possesses a state (x, y, theta) and its own prob
	
	printf("Number of particles %lu\n", p_particle.particle_count);

	long unsigned int n = p_particle.particle_count;
	float *weight = (float *)calloc(n, sizeof(float));
	float *cdf_weight = (float *)calloc(n, sizeof(float));
	float sum_weight = 0, sum_prob = 0, sum_prob_check = 0;;
	uniform_real_distribution<float> distribution(0.0,1.0);
	particle_type particle, temp_particle;

	new_hornetsoft_particle(&particle, n);
	new_hornetsoft_particle(&temp_particle, n);

	for(long unsigned int i = 0; i < n; i++){
		// printf("bbb\n");
		temp_particle.state[i] = sample_motion_model_odometry(p_odometry, odometry, p_particle.state[i]);
		// printf("%lu %lu %e\n", n, i, p_particle.prob[i]);
		// printf("p_state: %f %f %f\n", p_particle.state[i].x, p_particle.state[i].y, p_particle.state[i].theta);
		// printf("state: %f %f %f\n", temp_particle.state[i].x, temp_particle.state[i].y, temp_particle.state[i].theta);
		weight[i] = sensor_model(laser, temp_particle.state[i], map, param);
		sum_weight = sum_weight + weight[i]; //its summation needs not to be one yet!
		// printf("%lu %f %f\n", i, sum_weight, weight[i]);
		// printf("ccc\n");
	}
	// printf("%f %f\n", temp_particle.state[0].x, p_particle.state[0].x);
	// printf("%f %f\n", temp_particle.state[1].x, p_particle.state[1].x);
	// printf("%f %f\n", temp_particle.state[2000].x, p_particle.state[2000].x);
	// printf("%lu, %f\n", n, sum_weight);

	for(long unsigned int i = 0; i < n; i++){
		temp_particle.prob[i] = (p_particle.prob[i]*weight[i])/(sum_weight/n); //update&normalize prob
		if(i == 0){
			cdf_weight[i] = temp_particle.prob[i];
		}else{
			cdf_weight[i] = cdf_weight[i-1] + temp_particle.prob[i];
		}
		// printf("%f %f %f %e\n", p_particle.state[i].x, temp_particle.state[i].x, temp_particle.prob[i], cdf_weight[i]);		
	}
	
	particle.particle_count = n; //can be an output of another function for an adaptive particle number

	for(long unsigned int i = 0; i < particle.particle_count; i++){
		float number = distribution(mtd);
		// printf("%f\n", number);

		long unsigned int j = 0;
		if(number < cdf_weight[0]){
			j = 0;
		}else if(number > cdf_weight[n-1]){
			j = n;
		}else{
			while(number - cdf_weight[j] > 0.0){
				// printf("%f %lu\n", number - cdf_weight[j], j);
				j = j + 1;
				
			}
		}
		// printf("%f %lu\n",number, j);
		particle.state[i] = temp_particle.state[j];
		particle.prob[i] = temp_particle.prob[j];
		sum_prob = sum_prob + particle.prob[i];
	}

	for(long unsigned int i = 0; i < particle.particle_count; i++){
		particle.prob[i] = particle.prob[i]/sum_prob; //update&normalize prob
		sum_prob_check = sum_prob_check + particle.prob[i];
		// printf("%f %f %e\n", particle.state[i].x, particle.prob[i], sum_prob_check);		
	}

	return particle;
}

state_type expected_state(particle_type particle){
	state_type state;
	state.x = 0.0;
	state.y = 0.0;
	state.theta = 0.0;

	for (int i = 0; i < particle.particle_count; i++){ // sum prob != 1
		state.x = state.x + particle.prob[i]*particle.state[i].x;
		state.y = state.y + particle.prob[i]*particle.state[i].y;
		state.theta = state.theta + particle.prob[i]*particle.state[i].theta; // be careful with angle
	}

	return state;
}