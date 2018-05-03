#include "main.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	printf("Hello Lab4\n\n");
	clock_t start;
    double duration;
	
	/* Read Log Data */
	log_type log1, log2, train_log, test_log;
	string Log1_name = "../data/oakland_part3_am_rf.node_features";
	string Log2_name = "../data/oakland_part3_an_rf.node_features";
	read_log(Log1_name.c_str(), &log1);
	read_log(Log2_name.c_str(), &log2);
	pick_two_classes(log1, &train_log);
	pick_two_classes(log2, &test_log);

	try
	{
		/* Regret */
		// double regret, sum_loss_best_expert, sum_loss;
		// sum_loss_best_expert = min_loss(train_log);

		/********** GP with RBF **********/
		log_type GP_RBF_Train, GP_RBF_Test;

		// printf("Start Gaussian Process Regression with a Gaussian RBF kernel\n");
		// GP_RBF(train_log, test_log, &GP_RBF_Train, &GP_RBF_Test);
		// printf("Regret Gaussian Process Regression with a Gaussian RBF kernel: %.4f\n\n", sum_loss - sum_loss_best_expert);

		/********** Duration Measurement **********/
		printf("Starting Clock...\n");
		start = std::clock();

		/********** KLR **********/
		log_type KLR_Train, KLR_Test;

		printf("Start Kernelized Logistic Regresson with a Gaussian RBF kernel\n");
		KLR_RBF(train_log, test_log, &KLR_Train, &KLR_Test);
		// printf("Regret for Baysian Linear Regression: %.4f\n\n", sum_loss - sum_loss_best_expert);

		duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;  
		cout << "Duration: "<< duration <<" seconds \n\n";


		/********** GP with aaa **********/
		log_type GP_RQ_Train, GP_RQ_Test;

		// printf("Start Gaussian Process Regression with a Rational Quadratic kernel\n");
		// GP_RQ(train_log, test_log, &GP_RQ_Train, &GP_RQ_Test);
		// printf("Regret Neural Network: %.4f\n\n", sum_loss - sum_loss_best_expert);

		/********** Duration Measurement **********/
		// printf("Starting Clock...\n");
		// start = std::clock();

		// // put any function here to measure the time it takes

		// duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;  
		// cout << "Duration: "<< duration <<" seconds \n\n";

		//********** Visualization using PCL /**********/

		printf("Visalization\n");
		data_visualization(train_log, test_log, GP_RBF_Train, GP_RBF_Test, KLR_Train, KLR_Test, GP_RQ_Train, GP_RQ_Test);

		while(1){
			/* Does nothing but smiling at you :) */
			break;
		};
	}
	catch(const std::runtime_error& re)
	{
		// speciffic handling for runtime_error
		std::cerr << "Runtime error: " << re.what() << std::endl;
	}
	catch(const std::exception& ex)
	{
		// speciffic handling for all exceptions extending std::exception, except
		// std::runtime_error which is handled explicitly
		std::cerr << "Error occurred: " << ex.what() << std::endl;
	}
	catch(...)
	{
		// catch any other errors (that we have no information about)
		std::cerr << "Unknown failure occurred. Possible memory corruption" << std::endl;
	}

	return 0;
}