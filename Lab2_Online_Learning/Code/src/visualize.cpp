#include "visualize.hpp"
#include <iostream>
#include <stdlib.h>

#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>

boost::shared_ptr<pcl::visualization::PCLVisualizer> simpleVis (pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud, const char *view_name)
{
  // --------------------------------------------
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer (view_name));
  viewer->setBackgroundColor (0, 0, 0);
  viewer->addPointCloud<pcl::PointXYZ> (cloud, "cloud");
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "cloud");
  viewer->addCoordinateSystem (10.0);
  viewer->initCameraParameters ();
  return (viewer);
}

boost::shared_ptr<pcl::visualization::PCLVisualizer> rgbVis (pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud, const char *view_name)
{
  // --------------------------------------------
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer (view_name));
  viewer->setBackgroundColor (0, 0, 0); //black
  pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
  viewer->addPointCloud<pcl::PointXYZRGB> (cloud, rgb, "cloud");
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 0.5, "cloud");
  viewer->addCoordinateSystem (10.0);
  // viewer->initCameraParameters ();
  viewer->setCameraPosition(-123.354, -24.4749, 38.5989, 0.304104, -0.0311046, 0.952131);
  return (viewer);
}

int data_visualization(log_type log, const char *view_name){
  pcl::PointCloud<pcl::PointXYZ>::Ptr basic_cloud_ptr (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);

  float xm = 0, ym = 0, zm = 0;
  for(int i = 0; i < log.count; i++){
	xm = xm + log.point[i].x;
    ym = ym + log.point[i].y;
    zm = zm + log.point[i].z;
  }
  xm = xm / log.count;
  ym = ym / log.count;
  zm = zm / log.count;

  for(int i = 0; i < log.count; i++){
  	pcl::PointXYZ basic_point;
	basic_point.x = log.point[i].x - xm;
    basic_point.y = log.point[i].y - ym;
    basic_point.z = log.point[i].z - zm;
    basic_cloud_ptr->points.push_back(basic_point);

    pcl::PointXYZRGB point;
    point.x = basic_point.x;
    point.y = basic_point.y;
    point.z = basic_point.z;

    uint8_t r, g , b;
    if(log.node_label[i] == VEG){
    	r = 0, g = 255, b = 0;
    }else if(log.node_label[i] == WIRE){
    	r = 0, g = 0, b = 255;
    }else if(log.node_label[i] == POLE){
    	r = 255, g = 0, b = 0;
    }else if(log.node_label[i] == GROUND){
    	r = 255, g = 255, b = 0;
    }else if(log.node_label[i] == FACADE){
    	r = 255, g = 0, b = 255;
    }
    
	uint32_t rgb = ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b);
	point.rgb = *reinterpret_cast<float*>(&rgb);
	point_cloud_ptr->points.push_back (point);
  }
  basic_cloud_ptr->width = (int) basic_cloud_ptr->points.size ();
  basic_cloud_ptr->height = 1;
  point_cloud_ptr->width = (int) point_cloud_ptr->points.size ();
  point_cloud_ptr->height = 1;

  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
  // viewer = simpleVis(basic_cloud_ptr, view_name);
  viewer = rgbVis(point_cloud_ptr, view_name);

  // std::vector<pcl::visualization::Camera> cam; 
  while (!viewer->wasStopped ())
  {
    // viewer->spinOnce (100);
    // viewer->getCameras(cam); 
    // std::cout << "Cam: " << endl 
    //      << " - pos: (" << cam[0].pos[0] << ", "    << cam[0].pos[1] << ", "    << cam[0].pos[2] << ")" << endl 
    //      << " - view: ("    << cam[0].view[0] << ", "   << cam[0].view[1] << ", "   << cam[0].view[2] << ")"    << endl 
    //      << " - focal: ("   << cam[0].focal[0] << ", "  << cam[0].focal[1] << ", "  << cam[0].focal[2] << ")"   << endl
    //      << endl;
    boost::this_thread::sleep (boost::posix_time::microseconds (100000));
  }

}

// void save_image(Mat img, int step){
//    stringstream ss;
//    string filename;
//    string name = "../results/step_";
//    string type = ".jpg";

//    ss<<name<<(step)<<type;
//    filename = ss.str();
//    ss.str("");
//    imwrite(filename, img);
// }
