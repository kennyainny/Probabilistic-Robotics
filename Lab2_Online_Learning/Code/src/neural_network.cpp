#include "neural_network.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//Create a fully connected ANN with 12 inputs(IL), 4 hidden layer(HL) nodes and one output layer (OL)
float sigmoid(float number){
	float value;
	value=1/(1+exp(-number));
	return value;
}


void ANN(log_type log){
float weights_L1[48];
float weights_L2[4];
float input_nodes[12];
float hidden_nodes[4];
float output;
int i=0;
float a;
//Initiate weight values, 48 weights between IL and HL, 4 weights between HL and OL
//Initialize weights as random numbers between 0 and 1
for (int n=0;n<=47;n++){
a=rand()%100+1;
a=a/100;
weights_L1[n]=a;

}
for (int n=0;n<=3;n++){
a=rand()%100+1;
a=a/100;
weights_L2[n]=a;

}

//First, foward propogate 
//Intialize Input nodes

input_nodes[0]=log.point[i].x;
input_nodes[1]=log.point[i].y;
input_nodes[2]=log.point[i].z;
input_nodes[3]=log.node_label[i];
input_nodes[4]=log.feature[i].f[0];
input_nodes[5]=log.feature[i].f[1];
input_nodes[6]=log.feature[i].f[2];
input_nodes[7]=log.feature[i].f[3];
input_nodes[8]=log.feature[i].f[4];
input_nodes[9]=log.feature[i].f[5];
input_nodes[10]=log.feature[i].f[6];
input_nodes[11]=log.feature[i].f[7];
//Multiply weights by input nodes and su
int m=0;
for(int i=0; i<=3;i++){
	float sum=0;
	for(int n=0;n<=11;n++){
		sum=weights_L1[m]*input_nodes[n]+sum;
		// printf("printing");
		// printf("%i\n",m);
		m++;
	}
	hidden_nodes[i]=sum;

}
//Calculate actual node value by using sigmoid function
for(int i=0;i<=11;i++){
	// printf("BEFORE\n");
	// printf("%f\n",hidden_nodes[i]);
	hidden_nodes[i]=sigmoid(hidden_nodes[i]);
	// printf("After\n");
	// printf("%f\n",hidden_nodes[i]);
}
//go through second level of weights
float sum=0;
for(int i=0;i<=3;i++){
	sum=weights_L2[i]*hidden_nodes[i]+sum;

}
output=sum;
//Foward propogation complete
//How are we measuring convergence? If it's by error then put error code here

}


