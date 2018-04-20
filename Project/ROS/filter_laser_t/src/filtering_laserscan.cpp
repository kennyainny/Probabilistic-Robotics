#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include <sstream>

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float64MultiArray.h"

float total_range_vector [91*3];
void laserscanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	float range_vector [360];
	for (int i=0, j=0;i<=360;i++){
		range_vector[i]=msg->ranges[i];
		if (i<=45 || (i>=315 && i<360)){
			total_range_vector[j]=range_vector[i];
			if(total_range_vector[j]>1.75){
				printf("TOO LARGE \n");
				total_range_vector[j]=1.75;
			}

				printf("Scan Number %i\n",j);
				printf("Stuff %f\n",total_range_vector[j]);
			j++;
		}
	}
}

void laserscanCallback2(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	float range_vector [360];
	for (int i=0, j=91;i<=360;i++){
		range_vector[i]=msg->ranges[i];
		if (i<=45 || (i>=315 && i<360)){
			total_range_vector[j]=range_vector[i];
			if(total_range_vector[j]>1.75){
				printf("TOO LARGE \n");
				total_range_vector[j]=1.75;
			}

				printf("Scan Number %i\n",j);
				printf("Stuff %f\n",total_range_vector[j]);
			j++;
		}
	}
}


void laserscanCallback3(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	float range_vector [360];
	for (int i=0, j=182;i<=360;i++){
		range_vector[i]=msg->ranges[i];
		if (i<=45 || (i>=315 && i<360)){
			total_range_vector[j]=range_vector[i];
			if(total_range_vector[j]>1.75){
				printf("TOO LARGE \n");
				total_range_vector[j]=1.75;
			}

				printf("Scan Number %i\n",j);
				printf("Stuff %f\n",total_range_vector[j]);
			j++;
		}

	}

}


int main (int argc, char **argv)
{

	ros::init(argc,argv,"listener");
	ros::NodeHandle n;
	ros::NodeHandle nh;
	ros::NodeHandle nf;
	ros::NodeHandle np;
	ros::NodeHandle nd;
	// ros::Rate loop_rate(2);
	
	ros::Subscriber sub=n.subscribe("scan",1000,laserscanCallback);
	ros::Subscriber sub1=nh.subscribe("scan_2",1000,laserscanCallback2);
	ros::Subscriber sub2=nf.subscribe("scan_3",1000,laserscanCallback3);
	//ros::Publisher all_scan=np.advertise<sensor_msgs::LaserScan>("listener",1000);
	ros::Publisher chatter_pub = nd.advertise<std_msgs::Float64MultiArray>("chatter", 1000);
	//all_scan.publish(total_range_vector);
	while (ros::ok())
	{
	// std_msgs::String msg;
	// int count=1;
 //    std::stringstream ss;
 //    ss << "hello world " << count;
 //    msg.data = ss.str();

    std_msgs::Float64MultiArray laser_data;
    laser_data.data.clear();
    for(int i = 0; i < 91*3; i++){
    	laser_data.data.push_back(total_range_vector[i]);
    }

    // ROS_INFO("abc");
    chatter_pub.publish(laser_data);


    ros::spinOnce();

    // loop_rate.sleep();
    // ++count;
	}

	ros::spin();
	return 0;
	
}


