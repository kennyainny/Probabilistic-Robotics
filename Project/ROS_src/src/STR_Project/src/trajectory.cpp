#include "trajectory.hpp"

#define TIME_STEP 0.01 // sec/step

using namespace std;
using namespace Eigen;
float odom_x;
float odom_y;

void odomCallback(const nav_msgs::Odometry::ConstPtr& odom)
{
  ROS_INFO("I recieved odom: [%f,%f,%f]", odom->pose.pose.position.x,
    odom-> pose.pose.position.y,odom->pose.pose.position.z); //store x, y, z position values
  	odom_x=odom-> pose.pose.position.x;
  	odom_y=odom-> pose.pose.position.y;
}
int odom_main(int argcc, char **argvv)
{
	printf("Running \n");
	ros::init(argcc, argvv, "odom_listener");
	ros::NodeHandle n;
	ros::NodeHandle nh;
  	ros::Subscriber odom_sub = n.subscribe("odom", 1, odomCallback); //change to target_odom
  	printf("*************************************************\n");
 //  	ros::Publisher target_odom_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",1000);
 //  	printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
 //  	ros::Rate loop_rate(10);
 //  	int count = 0;
 //  	while (ros::ok())
 //  	{	
 //  		printf("##############################################\n");
 //  		//printf("ODOM %f\n",odom_x);
	//     geometry_msgs::Twist msg;
	//     msg.linear.x = 0.1;
	//     msg.angular.z = 0;    
	//     target_odom_pub.publish(msg);
	//     ros::spinOnce();
	//     loop_rate.sleep();
	//     ++count;
	// }
  	ros::spin();
	return 0;
}

Vector3d Simulate_Trajectory(Vector3d p_gt_old){
	Vector3d p(0, 0, OMEGA); //x , y, Omega
	printf("ODOM_X %f\n",odom_x);
	Matrix3d R;
	R = AngleAxisd(OMEGA*TIME_STEP, Vector3d::UnitZ());
	// R = AngleAxisd(RAD(90), Vector3d::UnitZ());
	p = R*p_gt_old;

	return p;
}




int move_robot(int argccc,char **argvvv){
	printf("Running \n");
	ros::init(argccc,argvvv,"trajectory");
	ros::NodeHandle nh;
	ros::Publisher target_odom_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",1000);
	//ros::Rate loop_rate(1);
	//printf("ODOM %f\n",odom_x);
	//geometry_msgs::Twist msg; 
	//x=msg->linear.x=1;
	int count = 0;
  	// while (ros::ok())
  	// {	
  		printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	    geometry_msgs::Twist msg;
	    msg.linear.x = 1;
	    msg.angular.z = 0;    
	    target_odom_pub.publish(msg);
	    // ros::spinOnce();
	    // loop_rate.sleep();
	    // ++count;
	// }
	ros::spin();
	return 0;


}
