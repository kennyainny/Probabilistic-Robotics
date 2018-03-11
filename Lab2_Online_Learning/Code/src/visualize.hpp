#ifndef VISUALIZE_HPP
#define VISUALIZE_HPP

extern "C"
{
	#include "def_type.h"
}

#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>

boost::shared_ptr<pcl::visualization::PCLVisualizer> simpleVis (pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud, const char *view_name);
boost::shared_ptr<pcl::visualization::PCLVisualizer> rgbVis (pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud, const char *view_name);
void create_point_cloud(log_type log, pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_ptr);
int data_visualization(log_type train_log, log_type test_log, log_type train_log_noise1, log_type train_log_noise2, 
					   log_type gradient_log_online, log_type gradient_log_stat, log_type gradient_online_log_noise1, log_type gradient_stat_log_noise1, log_type gradient_online_log_noise2, log_type gradient_stat_log_noise2,
					   log_type BLR_log_online, log_type BLR_log_stat, log_type BLR_online_log_noise1, log_type BLR_stat_log_noise1, log_type BLR_online_log_noise2, log_type BLR_stat_log_noise2,
					   log_type NN_log_online, log_type NN_log_stat, log_type NN_online_log_noise1, log_type NN_stat_log_noise1, log_type NN_online_log_noise2, log_type NN_stat_log_noise2);
// void save_image(Mat img, int step);

#endif /* VISUALIZE_HPP */