#include "sensor.h"
#include <stdio.h>

void new_hornetsoft_sensor(sensor_type *sensor, int size_x, int size_y)
{
  int i;

  sensor->prob = (float **)calloc(size_x, sizeof(float *));
  for(i = 0; i < size_x; i++)
    sensor->prob[i] = (float *)calloc(size_y, sizeof(float));
}

int read_beesoft_sensor(char *sensorName, sensor_type *sensor)
{
  int x, y, count;
  float temp;
  char line[256];
  FILE *fp;

  if((fp = fopen(sensorName, "rt")) == NULL) {
    fprintf(stderr, "# Could not open file %s\n", sensorName);
    return -1;
  }
  fprintf(stderr, "# Reading map: %s\n", sensorName);
  while((fgets(line, 256, fp) != NULL)
	&& (strncmp("global_map[0]", line , 13) != 0)) {
    if(strncmp(line, "robot_specifications->resolution", 32) == 0)
      if(sscanf(&line[32], "%d", &(sensor->resolution)) != 0)
	printf("# Map resolution: %d cm\n", sensor->resolution);
    if(strncmp(line, "robot_specifications->autoshifted_x", 35) == 0)
      if(sscanf(&line[35], "%g", &(sensor->offset_x)) != 0) {
	sensor->offset_x = sensor->offset_x;
	printf("# Map offsetX: %g cm\n", sensor->offset_x);
      }
    if(strncmp(line, "robot_specifications->autoshifted_y", 35) == 0) {
      if (sscanf(&line[35], "%g", &(sensor->offset_y)) != 0) {
	sensor->offset_y = sensor->offset_y;
	printf("# Map offsetY: %g cm\n", sensor->offset_y);
      }
    }
  }

  if(sscanf(line,"global_map[0]: %d %d", &sensor->size_y, &sensor->size_x) != 2) {
    fprintf(stderr, "ERROR: corrupted file %s\n", sensorName);
    fclose(fp);
    return -1;
  }
  printf("# Map size: %d %d\n", sensor->size_x, sensor->size_y);

  new_hornetsoft_sensor(sensor, sensor->size_x, sensor->size_y);

  sensor->min_x = sensor->size_x;
  sensor->max_x = 0;
  sensor->min_y = sensor->size_y;
  sensor->max_y = 0;
  count = 0;
  for(x = 0; x < map->size_x; x++)
    for(y = 0; y < map->size_y; y++, count++) {
      if(count % 10000 == 0)
	fprintf(stderr, "\r# Reading ... (%.2f%%)",
		count / (float)(sensor->size_x * sensor->size_y) * 100);
      
      fscanf(fp,"%e", &temp);
      if(temp < 0.0)
	sensor->prob[x][y] = -1;
      else {
	if(x < sensor->min_x)
	  sensor->min_x = x;
	else if(x > sensor->max_x)
	  sensor->max_x = x;
	if(y < sensor->min_y)
	  sensor->min_y = y;
	else if(y > sensor->max_y)
	  sensor->max_y = y;
	sensor->prob[x][y] = 1 - temp;	   
      }
    }
  fprintf(stderr, "\r# Reading ... (%.2f%%)\n\n",
	  count / (float)(sensor->size_x * sensor->size_y) * 100);
  fclose(fp);
  return 0;
}
