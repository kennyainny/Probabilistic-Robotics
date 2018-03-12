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

int data_visualization(log_type train_log, log_type test_log, log_type train_log_noise1, log_type train_log_noise2, 
             log_type gradient_log_online, log_type gradient_log_stat, log_type gradient_online_log_noise1, log_type gradient_stat_log_noise1, log_type gradient_online_log_noise2, log_type gradient_stat_log_noise2,
             log_type BLR_log_online, log_type BLR_log_stat, log_type BLR_online_log_noise1, log_type BLR_stat_log_noise1, log_type BLR_online_log_noise2, log_type BLR_stat_log_noise2,
             log_type NN_log_online, log_type NN_log_stat, log_type NN_online_log_noise1, log_type NN_stat_log_noise1, log_type NN_online_log_noise2, log_type NN_stat_log_noise2){
  
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

  /* Noise Courrupted */
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr train_noise1_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  create_point_cloud(train_log_noise1, train_noise1_cloud_ptr);
  view_name = "Add a Large Number of Random Features to Training Data";
  viewer = rgbVis(train_noise1_cloud_ptr, view_name.c_str());

  pcl::PointCloud<pcl::PointXYZRGB>::Ptr train_noise2_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  create_point_cloud(train_log_noise2, train_noise2_cloud_ptr);
  view_name = "Add a Large Number of Noise-Courrupted Original Training Data to Training Data";
  viewer = rgbVis(train_noise2_cloud_ptr, view_name.c_str());

  /* Gradient Descent */
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr gradient_online_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  create_point_cloud(gradient_log_online, gradient_online_cloud_ptr);
  view_name = "Gradient Descent on Squared Loss - Online Learning";
  viewer = rgbVis(gradient_online_cloud_ptr, view_name.c_str());  

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr gradient_stat_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(gradient_log_stat, gradient_stat_cloud_ptr);
  // view_name = "Gradient Descent on Squared - Statistical Learning";
  // viewer = rgbVis(gradient_stat_cloud_ptr, view_name.c_str());

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr gradient_online_noise1_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(gradient_online_log_noise1, gradient_online_noise1_cloud_ptr);
  // view_name = "Gradient Descent on Squared Loss with Noise1 - Online Learning";
  // viewer = rgbVis(gradient_online_noise1_cloud_ptr, view_name.c_str());  

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr gradient_stat_noise1_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(gradient_stat_log_noise1, gradient_stat_noise1_cloud_ptr);
  // view_name = "Gradient Descent on Squared Loss with Noise1 - Statistical Learning";
  // viewer = rgbVis(gradient_stat_noise1_cloud_ptr, view_name.c_str());

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr gradient_online_noise2_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(gradient_online_log_noise2, gradient_online_noise2_cloud_ptr);
  // view_name = "Gradient Descent on Squared Loss with Noise2 - Online Learning";
  // viewer = rgbVis(gradient_online_noise2_cloud_ptr, view_name.c_str());  

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr gradient_stat_noise2_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(gradient_stat_log_noise2, gradient_stat_noise2_cloud_ptr);
  // view_name = "Gradient Descent on Squared Loss with Noise2 - Statistical Learning";
  // viewer = rgbVis(gradient_stat_noise2_cloud_ptr, view_name.c_str()); 

  /* Bayesian Linear Regression */
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr BLR_online_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  create_point_cloud(BLR_log_online, BLR_online_cloud_ptr);
  view_name = "Bayesian Linear Regression - Online Learning";
  viewer = rgbVis(BLR_online_cloud_ptr, view_name.c_str());  

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr BLR_stat_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(BLR_log_stat, BLR_stat_cloud_ptr);
  // view_name = "Bayesian Linear Regression - Statistical Learning";
  // viewer = rgbVis(BLR_stat_cloud_ptr, view_name.c_str());

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr BLR_online_noise1_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(BLR_online_log_noise1, BLR_online_noise1_cloud_ptr);
  // view_name = "Bayesian Linear Regression with Noise1 - Online Learning";
  // viewer = rgbVis(BLR_online_noise1_cloud_ptr, view_name.c_str());  

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr BLR_stat_noise1_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(BLR_stat_log_noise1, BLR_stat_noise1_cloud_ptr);
  // view_name = "Bayesian Linear Regression with Noise1 - Statistical Learning";
  // viewer = rgbVis(BLR_stat_noise1_cloud_ptr, view_name.c_str());

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr BLR_online_noise2_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(BLR_online_log_noise2, BLR_online_noise2_cloud_ptr);
  // view_name = "Bayesian Linear Regression with Noise2 - Online Learning";
  // viewer = rgbVis(BLR_online_noise2_cloud_ptr, view_name.c_str());  

  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr BLR_stat_noise2_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(BLR_stat_log_noise2, BLR_stat_noise2_cloud_ptr);
  // view_name = "Bayesian Linear Regression with Noise2 - Statistical Learning";
  // viewer = rgbVis(BLR_stat_noise2_cloud_ptr, view_name.c_str()); 

  /* Neural Network */
  //Online learning without noise
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr NN_online_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  create_point_cloud(NN_log_online, NN_online_cloud_ptr);
  view_name = "Neural Network - Online Learning";
  viewer = rgbVis(NN_online_cloud_ptr, view_name.c_str());  

  //Statistical learning without noise
  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr NN_stat_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(NN_log_stat, NN_stat_cloud_ptr);
  // view_name = "Neural Network - Statistical Learning";
  // viewer = rgbVis(NN_stat_cloud_ptr, view_name.c_str());

  //Online learning with noise1
  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr NN_online_noise1_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(NN_online_log_noise1, NN_online_noise1_cloud_ptr);
  // view_name = "Neural Network with Noise1 - Online Learning";
  // viewer = rgbVis(NN_online_noise1_cloud_ptr, view_name.c_str());  

  //OStats learning with noise1
  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr NN_stat_noise1_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(NN_stat_log_noise1, NN_stat_noise1_cloud_ptr);
  // view_name = "Neural Network with Noise1 - Statistical Learning";
  // viewer = rgbVis(NN_stat_noise1_cloud_ptr, view_name.c_str());

  //Online Learning with noise 2
  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr NN_online_noise2_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(NN_online_log_noise2, NN_online_noise2_cloud_ptr);
  // view_name = "Neural Network with Noise2 - Online Learning";
  // viewer = rgbVis(NN_online_noise2_cloud_ptr, view_name.c_str());  

  //Stats learning with noise 2
  // pcl::PointCloud<pcl::PointXYZRGB>::Ptr NN_stat_noise2_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
  // create_point_cloud(NN_stat_log_noise2, NN_stat_noise2_cloud_ptr);
  // view_name = "Neural Network with Noise2 - Statistical Learning";
  // viewer = rgbVis(NN_stat_noise2_cloud_ptr, view_name.c_str()); 

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
