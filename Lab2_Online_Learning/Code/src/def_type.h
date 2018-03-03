#ifndef DEF_TYPE_H
#define DEF_TYPE_H

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
