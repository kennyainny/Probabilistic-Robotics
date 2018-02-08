#include "distribution.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float normal_dist(float x, float x_max, float m, float var){
	float p = 0;
	if(x > 0 && x < x_max){
		p = 1/sqrt(2*M_PI*var)*exp(-0.5*pow((x-m),2)/var);
	}
	return p;
}; // var = std^2

float exp_dist(float x, float x_max, float lambda){
	float p = 0;
	if(x > 0 && x < x_max){
		p = lambda*exp(-lambda*x);
	}
	return p;
};

float narrow_uniform_dist(float x, float x_max, float range){
	float p;
	if(x > x_max-range && x < x_max){
		p = 1/range;
	}
	return p;
};

float uniform_dist(float x, float x_max){
	float p;
	if(x > 0 && x < x_max){
		p = 1/x_max;
	}
	return p;
};