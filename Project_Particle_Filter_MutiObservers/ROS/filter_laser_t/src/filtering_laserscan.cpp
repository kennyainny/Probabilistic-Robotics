#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include <sstream>

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float64MultiArray.h"

using namespace std;

float total_range_vector[91*3];
float range_vec_1[91], range_vec_2[91], range_vec_3[91];
void laserscanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{	
	// cout << msg->header.frame_id << endl;
	string frame_id = msg->header.frame_id;
	string scan1 = "turtlebot1/base_scan";
	string scan2 = "turtlebot2/base_scan";
	string scan3 = "turtlebot3/base_scan";

	if(frame_id == scan1){
		for(int i = 0; i < 360; i++){
			if (i >=0 && i <= 45){
				if(msg->ranges[i] < 1.75){
					range_vec_1[45+i] = msg->ranges[i];
				}else{
					range_vec_1[45+i] = 1.75;
				}
			}else if(i >= 315 && i < 360){
				if(msg->ranges[i] < 1.75){
					range_vec_1[i-315] = msg->ranges[i];
				}else{
					range_vec_1[i-315] = 1.75;
				}
			}
		}
	}
	if(frame_id == scan2){
		for(int i = 0; i < 360; i++){
			if (i >=0 && i <= 45){
				if(msg->ranges[i] < 1.75){
					range_vec_2[45+i] = msg->ranges[i];
				}else{
					range_vec_2[45+i] = 1.75;
				}
			}else if(i >= 315 && i < 360){
				if(msg->ranges[i] < 1.75){
					range_vec_2[i-315] = msg->ranges[i];
				}else{
					range_vec_2[i-315] = 1.75;
				}
			}
		}
	}
	if(frame_id == scan3){
		for(int i = 0; i < 360; i++){
			if (i >=0 && i <= 45){
				if(msg->ranges[i] < 1.75){
					range_vec_3[45+i] = msg->ranges[i];
				}else{
					range_vec_3[45+i] = 1.75;
				}
			}else if(i >= 315 && i < 360){
				if(msg->ranges[i] < 1.75){
					range_vec_3[i-315] = msg->ranges[i];
				}else{
					range_vec_3[i-315] = 1.75;
				}
			}
		}
	}
}


int main (int argc, char **argv){

	ros::init(argc,argv,"listener");
	ros::NodeHandle n;
	ros::NodeHandle nh;
	ros::NodeHandle nf;
	ros::NodeHandle np;
	ros::NodeHandle nd;
	// ros::Rate loop_rate(2);
	
	ros::Subscriber sub=n.subscribe("scan",1000,laserscanCallback);
	ros::Publisher chatter_pub = nd.advertise<std_msgs::Float64MultiArray>("chatter", 1000);

	while (ros::ok()){
    	std_msgs::Float64MultiArray laser_data;
    	laser_data.data.resize(91*3);
    	// laser_data.data.clear();
    	for(int i = 0; i < 91*3; i++){
    		if(i < 91){
    			laser_data.data[i] = range_vec_1[i];
    		}
    		else if(i < 91*2){
    			laser_data.data[i] = range_vec_2[i-91];
    		}
    		else{
    			laser_data.data[i] = range_vec_3[i-91*2];
    		}
    	}
    	chatter_pub.publish(laser_data);
		ros::spinOnce();
	}

	ros::spin();
	return 0;
	
}


