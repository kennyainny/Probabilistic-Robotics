#ifndef DEF_TYPE_H
#define DEF_TYPE_H

typedef struct {
  float x, y, theta;
} state_type;

typedef struct {
  int resolution, size_x, size_y;
  float offset_x, offset_y;
  int min_x, max_x, min_y, max_y;
  float **prob;
} map_type;

typedef struct {
  float x, y, theta;
} vector_type;

typedef struct {
  vector_type v;
  float ts;
} odometry_type;

typedef struct {
  vector_type v1;
  vector_type v2;
  float r[180];
  float ts;
} laser_type;

typedef struct {
  laser_type *laser;
  odometry_type *odometry;
  int laser_count, odometry_count;
} sensor_type;

#endif /* DEF_TYPE_H */