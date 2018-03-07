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

	log_type log1_sort;
	sort_log(log1, &log1_sort);

	// printf("%lu\n", log1_sort.count);
	// for(int i = 0; i < log1_sort.count; i++){
	// 	std::cout << log1_sort.point[i].x << ' ';
	// 	std::cout << log1_sort.point[i].y << ' ';
	// 	std::cout << log1_sort.point[i].z << ' ';
	// 	std::cout << log1_sort.node_id[i] << ' ';
	// 	std::cout << log1_sort.node_label[i] << ' ';
	// 	for(int j = 0; j < 9; j++){
	// 		std::cout << log1_sort.feature[i].f[j] << ' ';
	// 	}
	// 	std::cout << log1_sort.feature[i].bias;
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

	/* Add Noise */
	log_type train_log_noise1, train_log_noise2;
	Add_Noise_1(train_log, &train_log_noise1); //Add a large number of random features
	Add_Noise_2(train_log, &train_log_noise2); //Add a large number of features that are noise corrupted versions of the features already in the data-set

	/* Gradient Descent on Squared Loss */
	log_type gradient_log, gradient_log_noise1, gradient_log_noise2;
	Gradient_Descent(log1_sort, test_log, &gradient_log);

	/* Baysian Linear Regression */
	log_type baysian_log, baysian_log_noise1, baysian_log_noise2;


	/* NN ? Logistic Regression ? SVM */
	log_type nn_log, nn_log_noise1, nn_log_noise2;
	// ANN(train_log);	

	log_type logistic_log, logistic_log_noise1, logistic_log_noise2;
	log_type svm_log, svm_log_noise1, svm_log_noise2;


	/* Visualization using PCL */
	data_visualization(train_log, test_log, train_log_noise1, train_log_noise2, gradient_log);

	while(1){
		/* Does nothing but smiling at you :) */
		break;
	};
	return 0;
}