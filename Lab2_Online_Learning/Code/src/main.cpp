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
	//Add_Noise_1(train_log, &train_log_noise1); //Add a large number of random features
	//Add_Noise_2(train_log, &train_log_noise2); //Add a large number of features that are noise corrupted versions of the features already in the data-set

<<<<<<< HEAD

	/* Gradient Descent on Squared Loss */
	log_type gradient_log, gradient_log_noise1, gradient_log_noise2;
	//Gradient_Descent(train_log_noise2, test_log, &gradient_log);
/********** Gradient Descent on Squared Loss **********/
=======
	/* Regret */
	double regret, sum_loss_best_expert[train_log.count] = {0};
	min_loss(train_log, sum_loss_best_expert);

	/********** Gradient Descent on Squared Loss **********/
>>>>>>> f93595a4815ab4a4d5c2a185d8a71c9e4bdb197c
	log_type gradient_log_online, gradient_log_stat;
	log_type gradient_online_log_noise1, gradient_online_log_noise2;
	log_type gradient_stat_log_noise1, gradient_stat_log_noise2;

<<<<<<< HEAD
	// regret = Gradient_Descent(train_log, test_log, &gradient_log_online, &gradient_log_stat);
	// printf("Regret Gradient Descent: %.4f\n", regret);
	// regret = Gradient_Descent(train_log_noise1, test_log, &gradient_online_log_noise1, &gradient_stat_log_noise1);
	// printf("Regret Gradient Descent with Noise1: %.4f\n", regret);
	// regret = Gradient_Descent(train_log_noise2, test_log, &gradient_online_log_noise2, &gradient_stat_log_noise2);
	// printf("Regret Gradient Descent with Noise2: %.4f\n", regret);
// >>>>>>> 596f34ae45d269a37e80525c72a3fd4e0637a5a1
=======
	regret = Gradient_Descent(train_log, test_log, &gradient_log_online, &gradient_log_stat);
	printf("Regret Gradient Descent: %.4f\n", regret);
	regret = Gradient_Descent(train_log_noise1, test_log, &gradient_online_log_noise1, &gradient_stat_log_noise1);
	printf("Regret Gradient Descent with Noise1: %.4f\n", regret);
	regret = Gradient_Descent(train_log_noise2, test_log, &gradient_online_log_noise2, &gradient_stat_log_noise2);
	printf("Regret Gradient Descent with Noise2: %.4f\n", regret);
>>>>>>> f93595a4815ab4a4d5c2a185d8a71c9e4bdb197c

	/********** Baysian Linear Regression **********/
	log_type baysian_log, baysian_log_noise1, baysian_log_noise2;


	/********** Neural Network **********/
	log_type nn_log, nn_log_noise1, nn_log_noise2;
	ANN(train_log);	

	log_type logistic_log, logistic_log_noise1, logistic_log_noise2;
	log_type svm_log, svm_log_noise1, svm_log_noise2;


	/* Visualization using PCL */
<<<<<<< HEAD
// <<<<<<< HEAD
	//data_visualization(train_log, test_log, train_log_noise1, train_log_noise2, gradient_log);
// =======
	// data_visualization(train_log, test_log, train_log_noise1, train_log_noise2, 
	// 				   gradient_log_online, gradient_log_stat, gradient_online_log_noise1, gradient_stat_log_noise1, gradient_online_log_noise2, gradient_stat_log_noise2);
// >>>>>>> 596f34ae45d269a37e80525c72a3fd4e0637a5a1
=======
	data_visualization(train_log, test_log, train_log_noise1, train_log_noise2, 
					   gradient_log_online, gradient_log_stat, gradient_online_log_noise1, gradient_stat_log_noise1, gradient_online_log_noise2, gradient_stat_log_noise2);
>>>>>>> f93595a4815ab4a4d5c2a185d8a71c9e4bdb197c

	while(1){
		/* Does nothing but smiling at you :) */
		break;
	};
	return 0;
}