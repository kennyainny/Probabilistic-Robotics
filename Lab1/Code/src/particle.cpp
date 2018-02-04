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

	// printf("%d\n", map.size_x);

	long unsigned int count_p = 0;
	int theta_interval = 45; //every 1 deg is too much
	int theta_number = 360/theta_interval;

	for(int i = 0; i <= map.size_x-1; i++){
		for(int j = 0; j <= map.size_y-1; j++){
			if(map.prob[i][j] > -1.0 && map.prob[i][j] < 1.0){
				count_p = count_p + 1;
			}
		}
	}

	particle->particle_count = count_p*theta_number;
	new_hornetsoft_particle(particle, particle->particle_count);

	// printf("%lu\n", count_p);	
	// printf("%lu\n", particle->particle_count);	

	count_p = 0;
	for(int i = 0; i < map.size_x; i++){
		for(int j = 0; j < map.size_y; j++){
			if(map.prob[i][j] > -1.0 && map.prob[i][j] < 1.0){
				for(int k = 0; k < theta_number; k++){
					particle->prob[count_p] = 1.0/particle->particle_count;
					particle->state[count_p].x = i;
					particle->state[count_p].y = j;
					particle->state[count_p].theta = k;
					count_p = count_p + 1;
				}
			}
		}
	}
	// printf("%e\n", particle->prob[count_p-1]);	
}

particle_type particle_filter(particle_type p_particle, laser_type laser, odometry_type p_odometry, odometry_type odometry)
{	//each particle possesses a state (x, y, theta) and its own prob
	
	printf("Number of particles %lu\n", p_particle.particle_count);

	long unsigned int n = p_particle.particle_count;
	float *weight = (float *)calloc(n, sizeof(float));
	float *cdf_weight = (float *)calloc(n, sizeof(float));
	float sum_weight = 0;
	uniform_real_distribution<float> distribution(0.0,1.0);
	particle_type particle, temp_particle;

	new_hornetsoft_particle(&particle, n);
	new_hornetsoft_particle(&temp_particle, n);

	for(long unsigned int i = 0; i < n; i++){
		temp_particle.state[i] = sample_motion_model_odometry(p_odometry, odometry, p_particle.state[i]);
		// printf("%lu %lu %e\n", n, i, p_particle.prob[i]);
		weight[i] = 0.5; //need an update from sensor model, which is a function of temp_particle.state[i] and odometry
		sum_weight = sum_weight + weight[i]; //its summation needs not to be one yet!
	}
	printf("%f %f\n", temp_particle.state[0].x, p_particle.state[0].x);
	printf("%f %f\n", temp_particle.state[1].x, p_particle.state[1].x);
	printf("%f %f\n", temp_particle.state[2000].x, p_particle.state[2000].x);
	// printf("%lu, %f\n", n, sum_weight);

	for(long unsigned int i = 0; i < n; i++){
		temp_particle.prob[i] = (p_particle.prob[i]*weight[i])/(sum_weight/n); //update&normalize prob
		if(i == 0){
			cdf_weight[i] = temp_particle.prob[i];
		}else{
			cdf_weight[i] = cdf_weight[i-1] + temp_particle.prob[i];
		}
		// printf("%e\n", cdf_weight[i]);		
	}
	
	particle.particle_count = n; //can be an output of another function for an adaptive particle number

	for(long unsigned int i = 0; i < particle.particle_count; i++){
		float number = distribution(mtd);
		// printf("%f\n", number);

		long unsigned int j = 0;
		if(number < cdf_weight[0]){
			j = 0;
		}else if(number > cdf_weight[n]){
			j = n;
		}else{
			while(number - cdf_weight[j] > 0){
				j = j + 1;
			}
		}
		particle.state[i] = temp_particle.state[j];
		particle.prob[i] = temp_particle.prob[j];
	}

	return particle;
}