/* Define to prevent recursive inclusion -------------------------------------*/
<<<<<<< HEAD
#ifndef __MAP_H
#define __MAP_H
=======
#ifndef MAP_H
#define MAP_H
>>>>>>> 769531312ee81a4a3a28d39ea559a7c713e35b4a

typedef struct {
  int resolution, size_x, size_y;
  float offset_x, offset_y;
  int min_x, max_x, min_y, max_y;
  float **prob;
} map_type;

<<<<<<< HEAD
void new_hornetsoft_map(map_type *map, int size_x, int size_y);
int read_beesoft_map(char *mapName, map_type *map);

#endif /* __MAP_H */
=======

// #ifdef __cplusplus
// extern "C" {
// #endif

void new_hornetsoft_map(map_type *map, int size_x, int size_y);
int read_beesoft_map(char *mapName, map_type *map);

// #ifdef __cplusplus
// }
// #endif

#endif /* MAP_H */
>>>>>>> 769531312ee81a4a3a28d39ea559a7c713e35b4a
