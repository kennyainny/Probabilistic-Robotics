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

boost::shared_ptr<pcl::visualization::PCLVisualizer> simpleVis (pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud);
boost::shared_ptr<pcl::visualization::PCLVisualizer> rgbVis (pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud);
boost::shared_ptr<pcl::visualization::PCLVisualizer> interactionCustomizationVis ();
int data_visualization(log_type log);
// void save_image(Mat img, int step);

#endif /* VISUALIZE_HPP */