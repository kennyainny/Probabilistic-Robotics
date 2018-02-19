#ifndef DEF_TYPE_H
#define DEF_TYPE_H

#define DEG(rad)  (rad*180.0/M_PI)
#define RAD(deg)  (deg*M_PI/180.0)
#define MAX_LASER 820
#define MAP_THRESHOLD 0.9
#define SENSOR_THRESHOLD 0.7

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
  float r[180];
} laser_type;

typedef struct {
  laser_type *laser;
  int *type; //0 = odometry, 1 = laser
  state_type *v;
  state_type *v1;
  float *ts;  
  unsigned long int laser_count, odometry_count, sensor_count;
} sensor_type;

typedef struct {
  state_type *state;
  float *prob;
  unsigned long int particle_count;
} particle_type;

#endif /* DEF_TYPE_H */