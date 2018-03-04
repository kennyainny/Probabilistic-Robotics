#include "main.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{

	printf("Hello Lab2\n");
	
	/* Read Log Data */
	log_type log1, log2, train_log, test_log;
	string Log1_name = "../data/oakland_part3_am_rf.node_features";
	string Log2_name = "../data/oakland_part3_an_rf.node_features";
	read_log(Log1_name.c_str(), &log1);
	read_log(Log2_name.c_str(), &log2);

	// printf("%lu\n", log1.count);
	// for(int i = 0; i < log1.count; i++){
	// 	std::cout << log1.point[i].x << ' ';
	// 	std::cout << log1.point[i].y << ' ';
	// 	std::cout << log1.point[i].z << ' ';
	// 	std::cout << log1.node_id[i] << ' ';
	// 	std::cout << log1.node_label[i] << ' ';
	// 	for(int j = 0; j < 9; j++){
	// 		std::cout << log1.feature[i].f[j] << ' ';
	// 	}
	// 	std::cout << log1.feature[i].bias;
	// 	std::cout << '\n';
	// }

	bool c(1); //change to 0 to switch between training and testing data set.
	if(c){
		train_log = log1;
		test_log = log2;
	} 
	else{
		train_log = log2;
		test_log = log1;
	}

	/* Gradient Descent on Squared Loss */
	log_type gradient_log;
	Gradient_Descent(train_log, test_log, &gradient_log);

	/* Baysian Linear Regression */



	/* NN ? Logistic Regression ? SVM */



	/* Visualization using PCL */
	data_visualization(train_log, test_log, gradient_log);

	while(1){
		/* Does nothing but smiling at you :) */
		break;
	};
	return 0;
}