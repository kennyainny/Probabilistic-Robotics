#ifndef VISUALIZE_HPP
#define VISUALIZE_HPP

#include "def_type.hpp"

using namespace Eigen;

void initialize_visualization(Vector3d p1, Vector3d p2, Vector3d p3);
void plot_ground_truth(Vector3d p_gt);
void plot_sensor(Vector3d p1, Vector3d p2, Vector3d p3);
void plot_particle(particle_type particle);
void visualization(Vector3d v_gt, Vector3d p1, Vector3d p2, Vector3d p3);

#endif /* VISUALIZE_HPP */