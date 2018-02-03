#ifndef SENSOR_H
#define SENSOR_H

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
} sensor_type;

void new_hornetsoft_sensor(sensor_type *sensor, int size_x, int size_y);
int read_beesoft_sensor(char *sensorName, sensor_type *sensor);

#endif /* SENSOR_H */