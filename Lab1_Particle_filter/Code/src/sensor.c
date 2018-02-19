#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float sensor_model(laser_type laser, state_type state, map_type map){
  float p = 0.0;
  float angle, x_ray_casting, y_ray_casting;
  state_type laser_state; //25 cm offset
  laser_state.theta = state.theta;
  laser_state.x = state.x + (25/map.resolution)*cos(laser_state.theta);
  laser_state.y = state.y + (25/map.resolution)*sin(laser_state.theta);

  if((int)laser_state.x < map.size_x && (int)laser_state.y < map.size_y && 
     (int)laser_state.x > 0 && (int)laser_state.y > 0 &&
      map.prob[(int)laser_state.x][(int)laser_state.y] >= 0 && map.prob[(int)laser_state.x][(int)laser_state.y] > SENSOR_THRESHOLD){

    for (int i = 0; i < 180; i++){
      angle = RAD((float)i) + laser_state.theta;
      x_ray_casting = laser_state.x + (laser.r[i]/map.resolution) * cos(angle - M_PI / 2);
      y_ray_casting = laser_state.y + (laser.r[i]/map.resolution) * sin(angle - M_PI / 2);

      if((int)x_ray_casting < map.size_x && (int)y_ray_casting < map.size_y && 
          (int)x_ray_casting > 0 && (int)y_ray_casting > 0 && 
          map.prob[(int)x_ray_casting][(int)y_ray_casting] > 0 && map.prob[(int)x_ray_casting][(int)y_ray_casting] < SENSOR_THRESHOLD){
          p = p + 1;
      }        
    }    
  }
  return p/180;
}

void new_hornetsoft_sensor(sensor_type *sensor, int size_l, int size_o)
{
  sensor->laser = (laser_type *)calloc(size_l, sizeof(laser_type));
  sensor->type = (int *)calloc(size_l + size_o, sizeof(int));
  sensor->v = (state_type *)calloc(size_l + size_o, sizeof(state_type));
  sensor->v1 = (state_type *)calloc(size_l + size_o, sizeof(state_type));
  sensor->ts = (float *)calloc(size_l + size_o, sizeof(float));
}

int read_beesoft_sensor(char *sensorName, sensor_type *sensor)
{
  long unsigned int count_l = 0, count_o = 0,count_s = 0, count = 0;
  int state_lo;
  float ftemp, _ftemp[3+3+180+1];
  float ftemp2, _ftemp2[3+1];
  char *temp  = malloc(sizeof(int));
  FILE *fp;

  if((fp = fopen(sensorName, "rt")) == NULL) {
    fprintf(stderr, "# Could not open file %s\n", sensorName);
    return -1;
  }
  fprintf(stderr, "# Reading sensor: %s\n", sensorName); 
  while(fscanf(fp,"%s", temp) != -1){
    if(strcmp(temp, "L") == 0){
      count_l = count_l + 1;
    }else if(strcmp(temp, "O") == 0){
      count_o = count_o + 1;
    }
  }
  count_s = count_l + count_o;
  sensor->laser_count = count_l;
  sensor->odometry_count = count_o;
  sensor->sensor_count = count_s;

  new_hornetsoft_sensor(sensor, count_l, count_o);

  count_l = -1, count_o = -1, count_s = -1;
  rewind(fp);
  while(fscanf(fp,"%s", temp) != -1){
    if(strcmp(temp, "L") == 0){
      state_lo = 1;
      count_l = count_l + 1;
    }else if(strcmp(temp, "O") == 0){
      state_lo = 0;
      count_o = count_o + 1;
    }

    count_s = count_s + 1;
    sensor->type[count_s] = state_lo;

    if(state_lo == 1){
      int i;
      for(i = 0; i < 3+3+180+1; i++){
        fscanf(fp,"%f",&ftemp);
        _ftemp[i] = ftemp;
      }

      sensor->v[count_s].x = _ftemp[0];
      sensor->v[count_s].y = _ftemp[1];
      sensor->v[count_s].theta = _ftemp[2];
      sensor->v1[count_s].x = _ftemp[3];
      sensor->v1[count_s].y = _ftemp[4];
      sensor->v1[count_s].theta = _ftemp[5];

      for(i = 6; i < 6+180; i++){
        sensor->laser[count_l].r[i-6] = _ftemp[i];
      }

      sensor->ts[count_s] = _ftemp[i];      
    }else{

      for(int i = 0; i < 3+1; i++){
        fscanf(fp,"%f",&ftemp2);
        _ftemp2[i] = ftemp2;
      }

      sensor->v[count_s].x = _ftemp2[0];
      sensor->v[count_s].y = _ftemp2[1];
      sensor->v[count_s].theta = _ftemp2[2];
      sensor->ts[count_s] = _ftemp[3]; 
    }
  }
  fclose(fp);
  return 0;
}