#!/usr/bin/env python
import rospy
import cv2
import numpy as np
import sys
import itertools
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image, CompressedImage
from sensor_msgs.msg import LaserScan
from std_msgs.msg import Float64


def callback(data):
	#minimum_range=float()
	ranges=data.ranges
	print ranges[0]
	# counter=0
	# counter1=0
	# maniprange=[0]*360
	# realranges=[1000]*360
	# while counter <=359:
	# 	maniprange[counter]=ranges[counter]
	# 	if maniprange[counter]!=0:
	# 		realranges[counter1]=maniprange[counter]
	# 		counter1=counter1+1
	# 	counter=counter+1

	# minrange=min(realranges)
	# rospy.loginfo(minrange)
	# pub=rospy.Publisher('min_distance',Float64, queue_size=1)
	# pub.publish(minrange)
	


def laser_distribution():
		print "running"
		rospy.init_node('laser_distribution', anonymous=True)
		rospy.Subscriber("/scan",LaserScan,callback)
		#rospy.Subscriber("/scan_filtered,"LaserScan,callback)
		rospy.spin()



if __name__=='__main__':
		laser_distribution()