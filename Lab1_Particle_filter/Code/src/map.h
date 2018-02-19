#ifndef MAP_H
#define MAP_H

#include "def_type.h"

void new_hornetsoft_map(map_type *map, int size_x, int size_y);
int read_beesoft_map(const char *mapName, map_type *map);

#endif /* MAP_H */
