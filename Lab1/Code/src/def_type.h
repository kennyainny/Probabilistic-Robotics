#ifndef DEF_TYPE_H
#define DEF_TYPE_H

#define PI    3.14159265359f
#define DEG(rad)  (rad*180.0/PI)
#define RAD(deg)  (deg*PI/180.0)

typedef struct {
  float x, y, theta;
} state_type;

typedef struct {
  int resolution, size_x, size_y;
  float offset_x, offset_y;
  int min_x, max_x, min_y, max_y;
  float **prob; // pointer to a pointer of a float variable
} map_type;

typedef struct {
  state_type v;
  float ts;
} odometry_type;

typedef struct {
  state_type v1;
  state_type v2;
  float r[180];
  float ts;
} laser_type;

typedef struct {
  laser_type *laser; // pointer decleration
  odometry_type *odometry;
  long unsigned int laser_count, odometry_count;
} sensor_type;

typedef struct {
  state_type *state;
  float *prob;
  long unsigned int particle_count;
} particle_type;

#endif /* DEF_TYPE_H */