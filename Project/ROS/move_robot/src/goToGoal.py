#!/usr/bin/env python
import rospy
import cv2
import math
import numpy as np
import sys
import itertools
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image, CompressedImage
from sensor_msgs.msg import LaserScan
from std_msgs.msg import Float64
import time
import os
from geometry_msgs.msg import Quaternion
import tf


###########################################
#Declaring global variables
obstacle_dat_range=Float64()
obstacle_dat_angle=int()
count=int()
count=0
msg=Twist()
yaw=float()
total_angle=0
##############################################################
#Read in waypoints
waypoints='wayPoints.txt'
pnt1xar=[0];
pnt1yar=[0];


def obstacle(ob_data):
	#bring in information from node publishing angle and  distance of nearest obstacle
	global theta,obstacle_dat_range,k,msg,yaw, obstacle_dat_angle
	obstacle_dat_angle=ob_data.x #int
	obstacle_dat_range=ob_data.y #float


def position(data):
	pi=math.pi
	global count,yaw,obstacle_dat_angle,obstacle_dat_range,total_angle,pnt1xar,pnt1yar
	k=0.1 #Gain used in proportional controller for rotation
	k1=0.3 #Gain used for linear.x controller
	###############################################################
	#Change waypoints
	pntx=pnt1xar[count]
	pnty=pnt1yar[count]
	###############################################################
	#calculate the position of the turtlebot in the world 
	x_dist_trav=data.pose.pose.position.x #distance in the x direction traveled
	y_dist_trav=data.pose.pose.position.y #distance in the y direction traveled
	quaternion=(data.pose.pose.orientation.x,data.pose.pose.orientation.y,data.pose.pose.orientation.z,data.pose.pose.orientation.w)
	euler=tf.transformations.euler_from_quaternion(quaternion)
	yaw=euler[2]*(180/pi) #1.5 is 90 degrees to the left-1.5 is 90 270 to the left. Yaw starts at zero
	if yaw<0:
		yaw=360+yaw
	pnt1x_error=pntx-x_dist_trav
	pnt1y_error=pnty-y_dist_trav
	distance_error=math.sqrt((pnt1x_error)**2+(pnt1y_error)**2)
	#angle_error=0



	##################################################################		


	print"regular"
	if pnt1x_error>0 and pnt1y_error<0 and distance_error>0.1:
		#CORRECT
		angle=(pnt1y_error)/(pnt1x_error)
		angle_error=math.atan(angle)*(180/pi)
		theta=360-abs(angle_error) #angle of the waypoint in robot vehicle fixed cord system
		parallel=theta-180
		#print"first"
		if yaw<parallel:
			total_angle=(yaw+abs(angle_error))*-1
		else:
			total_angle=360-yaw+angle_error #Must rotate this much to face waypoint
	elif pnt1x_error<0 and pnt1y_error<0 and distance_error>0.1:
		#correct
		angle=(pnt1x_error)/(pnt1y_error)
		angle_error=math.atan(angle)*(180/pi)
		theta=180+angle_error
		parallel=theta-180
		if yaw>parallel:
			total_angle=270-yaw-angle_error
		else:
			total_angle= 270-yaw-angle_error
		#print "second"
	elif pnt1x_error>0 and pnt1y_error>0 and distance_error>0.1:
		#cORRECT
		angle=(pnt1y_error)/(pnt1x_error)
		angle_error=math.atan(angle)*(180/pi)
		theta=angle_error #This is theta for this sector
		parallel=angle_error+180
		#print "third"
		if yaw<=parallel:
			total_angle=(yaw-angle_error)*-1
		else:
			total_angle=360-yaw+abs(angle_error) #Must rotate to this angle to face waypoint
	elif pnt1x_error<0 and pnt1y_error>0 and distance_error>0.1:
		#Correct
		angle=(pnt1x_error)/(pnt1y_error)
		angle_error=math.atan(angle) *(180/pi) #This is correct
		theta=90+abs(angle_error)
		parallel=theta+180
		if yaw<=theta or yaw>=parallel:
			if yaw>=theta:
				total_angle=theta+(360-yaw)
			else:
				total_angle=theta-yaw
		else:
			total_angle=(yaw-theta)*-1
	
	print"total_angle"
	print total_angle
	print "distance_error"
	print distance_error
	if abs(total_angle)<1 and distance_error<0.01:
		print "stopping"
		msg.linear.x=0
		msg.angular.z=0
		pub=rospy.Publisher('/turtlebot1/cmd_vel',Twist,queue_size=1)
		pub.publish(msg)
		count=count+1
		#print count
		time.sleep(5)
	if abs(total_angle)<1:
		k=0
	if distance_error<0.01:
		k1=0
	msg.angular.z=total_angle*k
	
	if abs(msg.angular.z)>1:
		msg.linear.x=0
	else:
		msg.linear.x=0*k1
	
	##################################################
	#Publish msg to move wheels
	print "foward speed"
	print msg.linear.x
	print "turn"
	print msg.angular.z


	pub=rospy.Publisher('/turtlebot1/cmd_vel',Twist,queue_size=1)

	pub.publish(msg)



def position2(data):
	pi=math.pi
	global count,yaw,obstacle_dat_angle,obstacle_dat_range,total_angle,pnt1xar,pnt1yar
	k=0.1 #Gain used in proportional controller for rotation
	k1=0.3 #Gain used for linear.x controller
	###############################################################
	#Change waypoints
	pntx=pnt1xar[count]
	pnty=pnt1yar[count]
	###############################################################
	#calculate the position of the turtlebot in the world 
	x_dist_trav=data.pose.pose.position.x #distance in the x direction traveled
	y_dist_trav=data.pose.pose.position.y #distance in the y direction traveled
	quaternion=(data.pose.pose.orientation.x,data.pose.pose.orientation.y,data.pose.pose.orientation.z,data.pose.pose.orientation.w)
	euler=tf.transformations.euler_from_quaternion(quaternion)
	yaw=euler[2]*(180/pi) #1.5 is 90 degrees to the left-1.5 is 90 270 to the left. Yaw starts at zero
	if yaw<0:
		yaw=360+yaw
	pnt1x_error=pntx-x_dist_trav
	pnt1y_error=pnty-y_dist_trav
	distance_error=math.sqrt((pnt1x_error)**2+(pnt1y_error)**2)
	#angle_error=0



	##################################################################		


	print"regular"
	if pnt1x_error>0 and pnt1y_error<0 and distance_error>0.1:
		#CORRECT
		angle=(pnt1y_error)/(pnt1x_error)
		angle_error=math.atan(angle)*(180/pi)
		theta=360-abs(angle_error) #angle of the waypoint in robot vehicle fixed cord system
		parallel=theta-180
		#print"first"
		if yaw<parallel:
			total_angle=(yaw+abs(angle_error))*-1
		else:
			total_angle=360-yaw+angle_error #Must rotate this much to face waypoint
	elif pnt1x_error<0 and pnt1y_error<0 and distance_error>0.1:
		#correct
		angle=(pnt1x_error)/(pnt1y_error)
		angle_error=math.atan(angle)*(180/pi)
		theta=180+angle_error
		parallel=theta-180
		if yaw>parallel:
			total_angle=270-yaw-angle_error
		else:
			total_angle= 270-yaw-angle_error
		#print "second"
	elif pnt1x_error>0 and pnt1y_error>0 and distance_error>0.1:
		#cORRECT
		angle=(pnt1y_error)/(pnt1x_error)
		angle_error=math.atan(angle)*(180/pi)
		theta=angle_error #This is theta for this sector
		parallel=angle_error+180
		#print "third"
		if yaw<=parallel:
			total_angle=(yaw-angle_error)*-1
		else:
			total_angle=360-yaw+abs(angle_error) #Must rotate to this angle to face waypoint
	elif pnt1x_error<0 and pnt1y_error>0 and distance_error>0.1:
		#Correct
		angle=(pnt1x_error)/(pnt1y_error)
		angle_error=math.atan(angle) *(180/pi) #This is correct
		theta=90+abs(angle_error)
		parallel=theta+180
		if yaw<=theta or yaw>=parallel:
			if yaw>=theta:
				total_angle=theta+(360-yaw)
			else:
				total_angle=theta-yaw
		else:
			total_angle=(yaw-theta)*-1
	
	print"total_angle"
	print total_angle
	print "distance_error"
	print distance_error
	if abs(total_angle)<1 and distance_error<0.01:
		print "stopping"
		msg.linear.x=0
		msg.angular.z=0
		pub=rospy.Publisher('/turtlebot2/cmd_vel',Twist,queue_size=1)
		pub.publish(msg)
		count=count+1
		#print count
		time.sleep(5)
	if abs(total_angle)<1:
		k=0
	if distance_error<0.01:
		k1=0
	msg.angular.z=total_angle*k
	
	if abs(msg.angular.z)>1:
		msg.linear.x=0
	else:
		msg.linear.x=0*k1
	
	##################################################
	#Publish msg to move wheels
	print "foward speed"
	print msg.linear.x
	print "turn"
	print msg.angular.z


	pub=rospy.Publisher('/turtlebot2/cmd_vel',Twist,queue_size=1)

	pub.publish(msg)
def position3(data):
	pi=math.pi
	global count,yaw,obstacle_dat_angle,obstacle_dat_range,total_angle,pnt1xar,pnt1yar
	k=0.1 #Gain used in proportional controller for rotation
	k1=0.3 #Gain used for linear.x controller
	###############################################################
	#Change waypoints
	pntx=pnt1xar[count]
	pnty=pnt1yar[count]
	###############################################################
	#calculate the position of the turtlebot in the world 
	x_dist_trav=data.pose.pose.position.x #distance in the x direction traveled
	y_dist_trav=data.pose.pose.position.y #distance in the y direction traveled
	quaternion=(data.pose.pose.orientation.x,data.pose.pose.orientation.y,data.pose.pose.orientation.z,data.pose.pose.orientation.w)
	euler=tf.transformations.euler_from_quaternion(quaternion)
	yaw=euler[2]*(180/pi) #1.5 is 90 degrees to the left-1.5 is 90 270 to the left. Yaw starts at zero
	if yaw<0:
		yaw=360+yaw
	pnt1x_error=pntx-x_dist_trav
	pnt1y_error=pnty-y_dist_trav
	distance_error=math.sqrt((pnt1x_error)**2+(pnt1y_error)**2)
	#angle_error=0



	##################################################################		


	print"regular"
	if pnt1x_error>0 and pnt1y_error<0 and distance_error>0.1:
		#CORRECT
		angle=(pnt1y_error)/(pnt1x_error)
		angle_error=math.atan(angle)*(180/pi)
		theta=360-abs(angle_error) #angle of the waypoint in robot vehicle fixed cord system
		parallel=theta-180
		#print"first"
		if yaw<parallel:
			total_angle=(yaw+abs(angle_error))*-1
		else:
			total_angle=360-yaw+angle_error #Must rotate this much to face waypoint
	elif pnt1x_error<0 and pnt1y_error<0 and distance_error>0.1:
		#correct
		angle=(pnt1x_error)/(pnt1y_error)
		angle_error=math.atan(angle)*(180/pi)
		theta=180+angle_error
		parallel=theta-180
		if yaw>parallel:
			total_angle=270-yaw-angle_error
		else:
			total_angle= 270-yaw-angle_error
		#print "second"
	elif pnt1x_error>0 and pnt1y_error>0 and distance_error>0.1:
		#cORRECT
		angle=(pnt1y_error)/(pnt1x_error)
		angle_error=math.atan(angle)*(180/pi)
		theta=angle_error #This is theta for this sector
		parallel=angle_error+180
		#print "third"
		if yaw<=parallel:
			total_angle=(yaw-angle_error)*-1
		else:
			total_angle=360-yaw+abs(angle_error) #Must rotate to this angle to face waypoint
	elif pnt1x_error<0 and pnt1y_error>0 and distance_error>0.1:
		#Correct
		angle=(pnt1x_error)/(pnt1y_error)
		angle_error=math.atan(angle) *(180/pi) #This is correct
		theta=90+abs(angle_error)
		parallel=theta+180
		if yaw<=theta or yaw>=parallel:
			if yaw>=theta:
				total_angle=theta+(360-yaw)
			else:
				total_angle=theta-yaw
		else:
			total_angle=(yaw-theta)*-1
	
	print"total_angle"
	print total_angle
	print "distance_error"
	print distance_error
	if abs(total_angle)<1 and distance_error<0.01:
		print "stopping"
		msg.linear.x=0
		msg.angular.z=0
		pub=rospy.Publisher('turtlebot3/cmd_vel',Twist,queue_size=1)
		pub.publish(msg)
		count=count+1
		#print count
		time.sleep(5)
	if abs(total_angle)<1:
		k=0
	if distance_error<0.01:
		k1=0
	msg.angular.z=total_angle*k
	
	if abs(msg.angular.z)>1:
		msg.linear.x=0
	else:
		msg.linear.x=0*k1
	
	##################################################
	#Publish msg to move wheels
	print "foward speed"
	print msg.linear.x
	print "turn"
	print msg.angular.z


	pub=rospy.Publisher('/turtlebot3/cmd_vel',Twist,queue_size=1)

	pub.publish(msg)

def goToGoal():
	rospy.init_node('goToGoal',anonymous=True)
	rospy.Subscriber("obstacle_info",Point,obstacle)
	rospy.Subscriber("/turtlebot1/odom",Odometry,position)
	rospy.Subscriber("/turtlebot2/odom",Odometry,position2)
	rospy.Subscriber("/turtlebot3/odom",Odometry,position3)
	print "goToGoal"
	rospy.spin()

if __name__=='__main__':
	goToGoal()