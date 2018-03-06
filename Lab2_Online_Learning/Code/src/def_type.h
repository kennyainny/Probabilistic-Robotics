#ifndef DEF_TYPE_H
#define DEF_TYPE_H

#define VEG    1004
#define WIRE   1100
#define POLE   1103
#define GROUND 1200
#define FACADE 1400

#define NOISE_RATIO 0.3 //# of noise data to # of original data

typedef struct {
  float x, y, z;
} point_type;

typedef struct {
  float f[9];
  float bias;
} feature_type;

typedef struct {
  point_type *point;
  long *node_id;
  int *node_label;
  feature_type *feature;
  long count;
} log_type;

#endif /* DEF_TYPE_H */
