/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAP_H
#define __MAP_H

typedef struct {
  int resolution, size_x, size_y;
  float offset_x, offset_y;
  int min_x, max_x, min_y, max_y;
  float **prob;
} map_type;

void new_hornetsoft_map(map_type *map, int size_x, int size_y);
int read_beesoft_map(char *mapName, map_type *map);

#endif /* __MAP_H */