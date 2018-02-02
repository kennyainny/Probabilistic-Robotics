<<<<<<< HEAD
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR_H
=======
/* Define to prevhent recursive inclusion -------------------------------------*/
#ifndef __SENSOR_
>>>>>>> 769531312ee81a4a3a28d39ea559a7c713e35b4a
#define __SENSOR_H

typedef struct {
  int resolution, size_x, size_y;
  float offset_x, offset_y;
  int min_x, max_x, min_y, max_y;
  float **prob;
} sensor_type;

void new_hornetsoft_map(sensor_type *sensor, int size_x, int size_y);
int read_beesoft_map(char *mapName, sensor_type *sensor);

#endif /* __SENSOR_H */