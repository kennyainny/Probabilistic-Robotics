#include "distribution.hpp"

float Normal_Dist(float x, float m, float sigma){
	float p = 1/sqrt(2*M_PI)/sigma*exp(-(x-m)*(x-m)/2/sigma/sigma);
	return p;
}

float Uniform_Dist(float x, float a, float b){
	float p = 0;
	if(x >= a && x <= b){
		p = 1/(b-a);
	}
	return p;
}

float Narrow_Uniform_Dist(float x, float x_max, float range){
	float p = 0;
	if(x >= x_max-range && x <= x_max){
		p = 1/(x_max-range);
	}
	return p;
}

float Map_Calc(float x, float in_min, float in_max, float out_min, float out_max){
	return (x - in_min)*(out_max - out_min)/(in_max - in_min) + out_min;
}