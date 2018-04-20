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

def movestuff(data):
	print "BEGAN"
	msg=Twist();
	msg.linear.x=0.9;
	msg.angular.z=1.2
	pub=rospy.Publisher('/target/cmd_vel',Twist,queue_size=1)
	pub.publish(msg)


def move():
	
	rospy.init_node('move',anonymous=True)
	rospy.Subscriber("/target/odom",Odometry,movestuff)
	rospy.spin()

if __name__=='__main__':
	try:		
		move()
	except rospy.ROSInterruptException:
		pass