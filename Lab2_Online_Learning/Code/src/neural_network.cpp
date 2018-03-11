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
	printf("Size of array %li\n",log.count);
	int NHN=6,NIN=10,NON=5,epochlim=20; //number of hidden layer nodes //numbre if input nodes//number of output nodes 
	int NWL1=NHN*NIN; //number of weights from input layer to hidden nodes
	int NWL2=NHN*NON; //number of weights from hidden layer to output
	float hidden_nodes[NHN],input_nodes[NIN],weights_L2[NWL2],weights_L1[NWL1],output[NON],error[NON];
	int reference[NON]; //Reference
	float real_error=0,a;

	//Initiate weight values, 48 weights between IL and HL, 4 weights between HL and OL
	//Initialize weights as random numbers between 0 and 1

	for (int n=0;n<NWL1;n++){
	// a=rand()%100+1;
	// a=a/100;
	float a=0;
	weights_L1[n]=a;
	}
	for (int n=0;n<NWL2;n++){
	// a=rand()%100+1;
	// a=a/100;
	float a=0;
	weights_L2[n]=a;
	}

	//Making weights into matrices
	float weights_L2_matrix[NON][NHN],weights_L1_matrix[NHN][NIN];
	for(int i=0,j=0,f=0;i<NWL1;i++,f++){
		if(f>NIN-1){
			f=0;
			j++;
		}
		weights_L1_matrix[j][f]=weights_L1[i]; //6x10
	}

	for (int i = 0, j=0,f=0; i < NWL2; ++i,f++){
		if (f>NON){
			f=0;
			j++;
		}
		weights_L2_matrix[j][f]=weights_L2[i]; //This is the correct size 5x6
	}



	for (int epoch=0;epoch<epochlim;epoch++){
		
		for (int q=0;q<log.count;q++){
			int a = 0;

			assign_output(log,q,reference,&a);
			printf("*************************\n");
			// for(int i=0;i<NWL2;i++){
			// 	// printf("Weights_L2 %f\n",weights_L2[i]);
			// }
				for(int i=0;i<NON;i++){
					printf("%i\n",reference[i]);
				}
			//First, foward propogate 
			//Intialize Input nodes
			// printf("WEIGHTS\n");
			// printf("%f\n",weights_L1[3]);
			// input_nodes[0]=log.point[q].x;
			// input_nodes[1]=log.point[q].y;
			// input_nodes[2]=log.point[q].z;
			//input_nodes[0]=log.node_label[q];
			input_nodes[0]=log.feature[q].f[0];
			input_nodes[1]=log.feature[q].f[1];
			input_nodes[2]=log.feature[q].f[2];
			input_nodes[3]=log.feature[q].f[3];
			input_nodes[4]=log.feature[q].f[4];
			input_nodes[5]=log.feature[q].f[5];
			input_nodes[6]=log.feature[q].f[6];
			input_nodes[7]=log.feature[q].f[7];
			input_nodes[8]=log.feature[q].f[8];
			input_nodes[9]=log.feature[q].bias;


			for(int k=0;k<NHN;k++){
				float sum=0;
				for(int i=0;i<10;i++){
					sum=weights_L1_matrix[k][i]*input_nodes[i]+sum; 
					// printf("weights_L2 %f\n",weights_L2_matrix[k][i]);
				}
				hidden_nodes[k]=sigmoid(sum);
				//printf("hidden_node values %f\n",hidden_nodes[k]);

			}
			
			//Second Level of weights and calculate error
			for(int k=0;k<NON;k++){
				float sum=0;
				for(int i=0;i<6;i++){
					sum=weights_L2_matrix[k][i]*hidden_nodes[i]+sum; 
					// printf("weights_L2 %f\n",weights_L2_matrix[k][i]);
				}
				output[k]=sigmoid(sum);
				printf("output value %f\n",output[k]);
				error[k]=reference[k]-output[k]; //dq value
			}
			
			//Foward propogation complete
		

			int alpha=1.5;
			//Begin back propogation

			
			//Calculate dp (matrix multiplication)
			float dp[4];
			int p = 0;
			for(int i=0;i<6;i++){
				float sum=0;
				for(int k=0;k<5;k++){
					sum=weights_L2_matrix[k][i]*error[k]+sum; 
					// printf("weights_L2 %f\n",weights_L2_matrix[k][i]);
					// printf("error value %f\n",error[k]);
				}
				dp[i]=sum;//should be a 6x1
				//printf("dp %f\n",dp[i]);
				
			}

			//calculate d_w2
			float d_w2[NON][NHN];
			for(int i=0,j=0,n=0,h=0,c=0;j<NON;h++,n++){
				if(n>NHN-1){
					n=0;
					//printf("dq %f\n",error[j]);
					j++;
				}
				if (h>NHN-1){
					h=0;
					c++;
					
				}
				d_w2[c][h]=alpha*error[j]*hidden_nodes[n]; //needs to be 5x6
				//printf("n %i\n",n);

			}

			//calculate d_w
			float d_w[NHN][NIN];
			for(int j=0, k=0,n=0,h=0,c=0;j<NHN;h++,n++){
				if (n>NIN-1){
					n=0;
					//printf("dp %f\n",dp[j]);
					j++;
				}
				if(h>NIN-1){
					h=0;
					c++;
				}
				d_w[c][h]=alpha*dp[j]*input_nodes[n]; //produces a 6x8 therefore 48 elements
				// printf("dp %f\n",d[i]);
				// printf("%f\n",d_w[i]);
			}

			// //Calculate new weights
			for(int n=0;n<NHN;n++){
				for(int p=0;p<NIN;p++){
					weights_L1_matrix[n][p]=d_w[n][p]+weights_L1_matrix[n][p];
					// printf(" set of weights %f\n", weights_L1[i]);
				}
			}

			for(int n=0;n<NON;n++){
				for(int p=0;p<NHN;p++){
					weights_L2_matrix[n][p]=d_w2[n][p]+weights_L2_matrix[n][p];
					// printf("second set of weights");
					// printf("%f\n",weights_L2[i]);
				}
			}
		}
	}

}