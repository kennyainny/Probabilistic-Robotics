#include "main.hpp"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{

	printf("Hello Lab2\n");
	
	log_type log1, log2;
	string Log1_name = "../data/oakland_part3_am_rf.node_features";
	string Log2_name = "../data/oakland_part3_an_rf.node_features";
	read_log(Log1_name, log1);
	read_log(Log2_name, log2);

	// waitkey(0); //uncomment this to not closing the figure
	while(1){
		/* Does nothing but smiling at you :) */
		break;
	};
	return 0;
}
