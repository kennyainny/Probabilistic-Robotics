#include "visualize.hpp"
#include <iostream>
#include <stdlib.h>

#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>

boost::shared_ptr<pcl::visualization::PCLVisualizer> simpleVis (pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud)
{
  // --------------------------------------------
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
  viewer->setBackgroundColor (0, 0, 0);
  viewer->addPointCloud<pcl::PointXYZ> (cloud, "sample cloud");
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");
  viewer->addCoordinateSystem (1.0);
  viewer->initCameraParameters ();
  return (viewer);
}

boost::shared_ptr<pcl::visualization::PCLVisualizer> rgbVis (pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud)
{
  // --------------------------------------------
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
  viewer->setBackgroundColor (0, 0, 0); //black
  pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
  viewer->addPointCloud<pcl::PointXYZRGB> (cloud, rgb, "sample cloud");
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud");
  viewer->addCoordinateSystem (1.0);
  viewer->initCameraParameters ();
  return (viewer);
}

int data_visualization(log_type log){
  // ------------------------------------
  // -----Create example point cloud-----
  // ------------------------------------
  pcl::PointCloud<pcl::PointXYZ>::Ptr basic_cloud_ptr (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  std::cout << "Genarating example point clouds.\n\n";
  // We're going to make an ellipse extruded along the z-axis. The colour for
  // the XYZRGB cloud will gradually go from red to green to blue.
  // uint8_t r(255), g(15), b(15);
  // for (float z(-1.0); z <= 1.0; z += 0.05)
  // {
  //   for (float angle(0.0); angle <= 360.0; angle += 5.0)
  //   {
  //     pcl::PointXYZ basic_point;
  //     basic_point.x = 0.8 * cosf (pcl::deg2rad(angle));
  //     basic_point.y = sinf (pcl::deg2rad(angle));
  //     basic_point.z = z;
  //     basic_cloud_ptr->points.push_back(basic_point);

  //     pcl::PointXYZRGB point;
  //     point.x = basic_point.x;
  //     point.y = basic_point.y;
  //     point.z = basic_point.z;
  //     uint32_t rgb = (static_cast<uint32_t>(r) << 16 |
  //             static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(b));
  //     point.rgb = *reinterpret_cast<float*>(&rgb);
  //     point_cloud_ptr->points.push_back (point);
  //   }
  //   if (z < 0.0)
  //   {
  //     r -= 12;
  //     g += 12;
  //   }
  //   else
  //   {
  //     g -= 12;
  //     b += 12;
  //   }
  // }
  // basic_cloud_ptr->width = (int) basic_cloud_ptr->points.size ();
  // basic_cloud_ptr->height = 1;
  // point_cloud_ptr->width = (int) point_cloud_ptr->points.size ();
  // point_cloud_ptr->height = 1;

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
  }
  basic_cloud_ptr->width = (int) basic_cloud_ptr->points.size ();
  basic_cloud_ptr->height = 1;

  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
  viewer = simpleVis(basic_cloud_ptr);
  // viewer = rgbVis(basic_cloud_ptr);

  //--------------------
  // -----Main loop-----
  //--------------------
  while (!viewer->wasStopped ())
  {
    viewer->spinOnce (100);
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
