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


void ANN( log_type train_log, log_type test_log, log_type *NN_log_online, log_type *NN_log_stat){
	NN_log_stat->count = train_log.count;  
 	new_hornetsoft_log(NN_log_stat);
	int NHN=7,NIN=10,NON=5,epochlim=300; //number of hidden layer nodes //numbre if input nodes//number of output nodes 
	int NWL1=NHN*NIN; //number of weights from input layer to hidden nodes
	int NWL2=NHN*NON; //number of weights from hidden layer to output
	double hidden_nodes[NHN],input_nodes[NIN],weights_L2[NWL2],weights_L1[NWL1],output[NON],error[NON];
	int reference[NON]; //Reference
	float real_error=0,a;

	//Initiate weight values, 48 weights between IL and HL, 4 weights between HL and OL
	//Initialize weights as random numbers between 0 and 1

	for (int n=0;n<NWL1;n++){
	a=rand()%100+1;
	a=a/100;
	//float a=0;
	weights_L1[n]=a;
	}
	for (int n=0;n<NWL2;n++){
	a=rand()%100+1;
	a=a/100;
	//float a=0;
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
		if (f>NHN-1){
			f=0;
			j++;
		}
		weights_L2_matrix[j][f]=weights_L2[i]; //This is the correct size 5x6
	}

	log_type train_log_sort;
  	sort_log(train_log, &train_log_sort);

	for (int epoch=0;epoch<epochlim;epoch++){
		// for(int n=0;n<NHN;n++){
		// 		for(int p=0;p<NIN;p++){
		// 			//weights_L1_matrix[n][p]=d_w[n][p]+weights_L1_matrix[n][p];
		// 			//printf(" set of weights %f\n", weights_L1_matrix[n][p]);
		// 		}
		// 	}
		//printf("*******************************\n");
		float errorsum=0;
		float smallerror=0;
		//float errorsumq=0;
		float mediumerror=0;
		for (int q=0;q<train_log.count;q++){
			
			int a = 0;
			float errorsumq=0;
			assign_output(train_log,q,reference,&a);
			// if(reference[0]!=1){
			// 	for (int d=0;d<NON-1;d++){
			// 		reference[d]=0;
			// 	}
			// }
			// while (reference[0]!=1){
			// 	q++;
			// 	assign_output(log,q,reference,&a);
			// }
			//printf("VALUE OF Q %i\n",q);


			//printf("*************************\n");
		
			// for(int i=0;i<NON;i++){
			// 	printf("%i\n",reference[i]);
			// }
			//First, foward propogate 
			//Intialize Input nodes
			// input_nodes[0]=log.point[q].x;
			// input_nodes[1]=log.point[q].y;
			// input_nodes[2]=log.point[q].z;
			//input_nodes[0]=log.node_label[q];
			input_nodes[0]=train_log.feature[q].f[0];
			input_nodes[1]=train_log.feature[q].f[1];
			input_nodes[2]=train_log.feature[q].f[2];
			input_nodes[3]=train_log.feature[q].f[3];
			input_nodes[4]=train_log.feature[q].f[4];
			input_nodes[5]=train_log.feature[q].f[5];
			input_nodes[6]=train_log.feature[q].f[6];
			input_nodes[7]=train_log.feature[q].f[7];
			input_nodes[8]=train_log.feature[q].f[8];
			input_nodes[9]=train_log.feature[q].bias;



			for(int k=0;k<NHN;k++){
				float sum=0;
				for(int i=0;i<10;i++){
					sum=weights_L1_matrix[k][i]*input_nodes[i]+sum; 
					// printf("weights_L2 %f\n",weights_L2_matrix[k][i]);
				}
				hidden_nodes[k]=sigmoid(sum);
			}

			//Second Level of weights and calculate error
			for(int k=0;k<NON;k++){
				float sum=0;
				for(int i=0;i<NHN-1;i++){
					sum=weights_L2_matrix[k][i]*hidden_nodes[i]+sum; //(
					// printf("weights_L2 %f\n",weights_L2_matrix[k][i]);
				}
				output[k]=sigmoid(sum);
				// printf("output value %f\n",output[k]);
				error[k]=reference[k]-output[k]; //dq value
				//printf("little error %f\n",error[k]);
				errorsumq=error[k]+errorsumq;
				// printf("errorsumq %f\n",errorsumq);

			}
			mediumerror=mediumerror+errorsumq;
			// printf("MEDIUM ERROR %f\n",mediumerror);


			//printf("ERROR of this run %f\n", errorsumq);
			//printf("Error %f\n",errorsum);
			
			//Foward propogation complete
		

			int alpha=1;
			//Begin back propogation

			
			//Calculate dp (matrix multiplication)
			float dp[NHN];
			int p = 0;
			for(int i=0;i<NHN-1;i++){
				float sum=0;
				for(int k=0;k<NON;k++){
					sum=weights_L2_matrix[k][i]*error[k]+sum; //(6x5)(5x1)
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
				//printf("d_w2 %f\n",d_w2[c][h]);
				//printf("hidden nodes %f\n",hidden_nodes[n]);
				// printf("eror %f\n",error[j]);

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
				//printf("d_w %f\n",d_w[c][h]);
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
					//printf("%f\n",weights_L2_matrix[n][p]);
					// printf("%f\n",weights_L2[i]);
				}
			}
		}
		smallerror=mediumerror/10;
		//printf("TOTAL Error %f\n",smallerror);
	}



	//printf("BEGIN TESTING!!!!!!!!!!!!!!!\n");
	
		
	for (int q=0;q<train_log.count;q++){
		NN_log_stat->point[q] = train_log.point[q];
    		NN_log_stat->node_id[q] = train_log.node_id[q];
		int a = 0;
		assign_output(train_log,q,reference,&a);
		// if(reference[0]!=1){
		// 	for (int d=0;d<4;d++){
		// 		reference[d]=0;
		// 	}
		// }
		//printf("TEST\n");
		for(int i=0;i<NON;i++){
			printf("%i\n",reference[i]);
		}
		// //First, foward propogate 
		//Intialize Input nodes
		// printf("WEIGHTS\n");
		// printf("%f\n",weights_L1[3]);
		// input_nodes[0]=log.point[q].x;
		// input_nodes[1]=log.point[q].y;
		// input_nodes[2]=log.point[q].z;
		//input_nodes[0]=log.node_label[q];
		input_nodes[0]=train_log.feature[q].f[0];
		input_nodes[1]=train_log.feature[q].f[1];
		input_nodes[2]=train_log.feature[q].f[2];
		input_nodes[3]=train_log.feature[q].f[3];
		input_nodes[4]=train_log.feature[q].f[4];
		input_nodes[5]=train_log.feature[q].f[5];
		input_nodes[6]=train_log.feature[q].f[6];
		input_nodes[7]=train_log.feature[q].f[7];
		input_nodes[8]=train_log.feature[q].f[8];
		input_nodes[9]=train_log.feature[q].bias;



		for(int k=0;k<NHN;k++){
			float sum=0;
			for(int i=0;i<NIN;i++){
				sum=weights_L1_matrix[k][i]*input_nodes[i]+sum; 
				//printf("Weights %f\n",weights_L1_matrix[k][i]);
			}
			hidden_nodes[k]=sigmoid(sum);
			//printf("Hidden Nodes %f\n",hidden_nodes[k]);
		}

		//Second Level of weights and calculate error
		for(int k=0;k<NON;k++){
			float sum=0;
			for(int i=0;i<NHN-1;i++){
				sum=weights_L2_matrix[k][i]*hidden_nodes[i]+sum; 
				//printf("weights_L2 %f\n",weights_L2_matrix[k][i]);
			}
			output[k]=sigmoid(sum);
			printf("output value %f\n",output[k]);
			error[k]=reference[k]-output[k]; //dq value
		}
		
		predict_label(NN_log_stat, output, q);
	}
	
}

double ANN_ONLINE(log_type train_log, log_type test_log, log_type *NN_log_online, log_type *NN_log_stat){
	NN_log_online->count = train_log.count;  
 	new_hornetsoft_log(NN_log_online);
	int NHN=6,NIN=10,NON=5,epochlim=1; //number of hidden layer nodes //numbre if input nodes//number of output nodes 
	int NWL1=NHN*NIN; //number of weights from input layer to hidden nodes
	int NWL2=NHN*NON; //number of weights from hidden layer to output
	double hidden_nodes[NHN],input_nodes[NIN],weights_L2[NWL2],weights_L1[NWL1],output[NON],error[NON];
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

	float errorsum=0;
	float smallerror=0;
	//float errorsumq=0;
	float mediumerror=0;

	for (int epoch=0;epoch<epochlim;epoch++){
		
		for (int q=0;q<train_log.count;q++){
			float errorsumq=0;
			int a = 0;
			NN_log_online->point[q] = train_log.point[q];
    		NN_log_online->node_id[q] = train_log.node_id[q];

			assign_output(train_log,q,reference,&a);
			// printf("*************************\n");
			// for(int i=0;i<NWL2;i++){
			// 	// printf("Weights_L2 %f\n",weights_L2[i]);
			// }
				// for(int i=0;i<NON;i++){
				// 	// printf("%i\n",reference[i]);
				// }
			// printf("Q VALUE %i\n",q);
			//First, foward propogate 
			//Intialize Input nodes
			// printf("WEIGHTS\n");
			// printf("%f\n",weights_L1[3]);
			// input_nodes[0]=log.point[q].x;
			// input_nodes[1]=log.point[q].y;
			// input_nodes[2]=log.point[q].z;
			//input_nodes[0]=log.node_label[q];
			input_nodes[0]=train_log.feature[q].f[0];
			input_nodes[1]=train_log.feature[q].f[1];
			input_nodes[2]=train_log.feature[q].f[2];
			input_nodes[3]=train_log.feature[q].f[3];
			input_nodes[4]=train_log.feature[q].f[4];
			input_nodes[5]=train_log.feature[q].f[5];
			input_nodes[6]=train_log.feature[q].f[6];
			input_nodes[7]=train_log.feature[q].f[7];
			input_nodes[8]=train_log.feature[q].f[8];
			input_nodes[9]=train_log.feature[q].bias;


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
				// printf("output value %f\n",output[k]);
				error[k]=reference[k]-output[k]; //dq value
				//printf("little error %f\n",error[k]);
				errorsumq=error[k]*error[k]+errorsumq;
				// printf("errorsumq %f\n",errorsumq);
			}
			mediumerror=mediumerror+errorsumq;

			
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
			predict_label(NN_log_online, output, q);
		}
	}
	smallerror=mediumerror/2;
	// printf("ERROR YOU WANT %f\n",smallerror);

	return smallerror;
}			







