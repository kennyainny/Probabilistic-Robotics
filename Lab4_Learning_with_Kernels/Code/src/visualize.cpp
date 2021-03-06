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

void create_point_cloud(log_type log, pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_ptr){
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

    pcl::PointXYZRGB point;
    point.x = log.point[i].x - xm;
    point.y = log.point[i].y - ym;
    point.z = log.point[i].z - zm;

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
  point_cloud_ptr->width = (int) point_cloud_ptr->points.size ();
  point_cloud_ptr->height = 1;
}

int data_visualization(log_type train_log, log_type test_log, log_type GP_RBF_Train, log_type GP_RBF_Test,
                       log_type KLR_Train, log_type KLR_Test, log_type GP_RQ_Train, log_type GP_RQ_Test){
  
  /* Original Data */
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr train_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  create_point_cloud(train_log, train_cloud_ptr);
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
  std::string view_name = "Original Training Data";
  viewer = rgbVis(train_cloud_ptr, view_name.c_str());

  pcl::PointCloud<pcl::PointXYZRGB>::Ptr test_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  create_point_cloud(test_log, test_cloud_ptr);
  view_name = "Original Testing Data";
  viewer = rgbVis(test_cloud_ptr, view_name.c_str());

  /* GP_RBF */
  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr gradient_online_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(gradient_log_online, gradient_online_cloud_ptr);
  // view_name = "Gradient Descent on Squared Loss - Online Learning";
  // viewer = rgbVis(gradient_online_cloud_ptr, view_name.c_str());  

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr GP_RBF_Test_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(GP_RBF_Test, GP_RBF_Test_cloud_ptr);
  // view_name = "Gaussian Process Regression with a Gaussian RBF kernel - Testing Data";
  // viewer = rgbVis(GP_RBF_Test_cloud_ptr, view_name.c_str());

  /* Bayesian Linear Regression */
  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr BLR_online_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(BLR_log_online, BLR_online_cloud_ptr);
  // view_name = "Bayesian Linear Regression - Online Learning";
  // viewer = rgbVis(BLR_online_cloud_ptr, view_name.c_str());  

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr BLR_stat_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(BLR_log_stat, BLR_stat_cloud_ptr);
  // view_name = "Bayesian Linear Regression - Statistical Learning";
  // viewer = rgbVis(BLR_stat_cloud_ptr, view_name.c_str());

  /* GP_RQ */
  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr gradient_online_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(gradient_log_online, gradient_online_cloud_ptr);
  // view_name = "Gradient Descent on Squared Loss - Online Learning";
  // viewer = rgbVis(gradient_online_cloud_ptr, view_name.c_str());  

  pcl::PointCloud<pcl::PointXYZRGB>::Ptr KLR_Test_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  create_point_cloud(KLR_Test, KLR_Test_cloud_ptr);
  view_name = "Kernel Logistic Regression with a RBF kernel - Testing Data";
  viewer = rgbVis(KLR_Test_cloud_ptr, view_name.c_str());


  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr GP_RQ_Test_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(GP_RQ_Test, GP_RQ_Test_cloud_ptr);
  // view_name = "Gaussian Process Regression with a Rational Quadratic kernel - Testing Data";
  // viewer = rgbVis(GP_RQ_Test_cloud_ptr, view_name.c_str());

  // std::vector<pcl::visualization::Camera> cam; 
  while (!viewer->wasStopped ())
  {
    viewer->spinOnce (100);
  //   // viewer->getCameras(cam); 
  //   // std::cout << "Cam: " << endl 
  //   //      << " - pos: (" << cam[0].pos[0] << ", "    << cam[0].pos[1] << ", "    << cam[0].pos[2] << ")" << endl 
  //   //      << " - view: ("    << cam[0].view[0] << ", "   << cam[0].view[1] << ", "   << cam[0].view[2] << ")"    << endl 
  //   //      << " - focal: ("   << cam[0].focal[0] << ", "  << cam[0].focal[1] << ", "  << cam[0].focal[2] << ")"   << endl
  //   //      << endl;
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
