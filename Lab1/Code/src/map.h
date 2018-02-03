#ifndef MAP_H
#define MAP_H

typedef struct {
  int resolution, size_x, size_y;
  float offset_x, offset_y;
  int min_x, max_x, min_y, max_y;
  float **prob;
} map_type;

// #ifdef __cplusplus
// extern "C" {
// #endif

void new_hornetsoft_map(map_type *map, int size_x, int size_y);
int read_beesoft_map(char *mapName, map_type *map);

// #ifdef __cplusplus
// }
// #endif

#endif /* MAP_H */
