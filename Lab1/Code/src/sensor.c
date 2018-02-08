#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>



//don't know if this is the best place to put it since whole program may iterate
float p_hit=0,q=1;

//Must imput range scan, robot pose and map
void z_ks(map_type map, state_type p_state){
  //printf( "%d\n",map.max_x);
  printf("break \n");
  printf("%f\n",*map.prob[3]);
  // printf("break \n");
  // printf("%f\n",p_state);
  // printf("z_ks calculated \n");

  
}
void sensor_model(map_type map){//,laser_type laser,state_type p_state){
  // for each given xi, z* will be changed
  //z_kp, z_k and sig_hit should be dynamic variables that change each iteration
  float p_rand,p,z_hit,z_short,z_max,z_rand,u_norm_dist,sig_hit,lamb_short,p_short,z_k,z_kp,normalizer_uo;
  int p_max;
  z_hit=0.25;
  z_short=0.25;
  z_max=0.25;
  z_rand=0.25;
  lamb_short=1;

  //delete these
  // z_k=1;
  // z_kp=1;
  if (z_k>=0 && z_k<=z_max ){
    u_norm_dist=(1/sqrt(2*M_PI*sig_hit)*exp(-0.5*pow((z_k-z_kp),2)/(pow(sig_hit,2))));
    p_hit=u_norm_dist+p_hit;
    p_rand=1/z_max;
  }
  else {
    p_hit=0;
    p_rand=0;
  }
  if (z_k>=0 && z_k<=z_kp){
    normalizer_uo=1/(1-exp(-lamb_short*z_kp));
    p_short=normalizer_uo*lamb_short*exp(exp(-lamb_short*z_k));
  }
  else{
    p_short=0;
  }
  if(z_k==z_max){
    p_max=1;
  }
  else{
    p_max=0;
  }
  p=z_hit*p_hit+z_short*p_short+z_max*p_max+z_rand*p_rand;
  q=q*p;
  printf("Sensor model complete \n");
  // printf("u_norm_dist \n");
  // printf("%f",u_norm_dist);
  //Implement Failures(max range error)
}

void new_hornetsoft_sensor(sensor_type *sensor, int size_l, int size_o)
{
  // sensor->laser = (laser_type *)malloc(size_l);
  // sensor->odometry = (odometry_type *)malloc(size_o);

  sensor->laser = (laser_type *)calloc(size_l, sizeof(laser_type));
  sensor->odometry = (odometry_type *)calloc(size_o, sizeof(odometry_type));
}

int read_beesoft_sensor(char *sensorName, sensor_type *sensor)
{
  long unsigned int count_l = 0, count_o = 0, count = 0;
  int state_lo;
  float ftemp, _ftemp[3+3+180+1];
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
  sensor->laser_count = count_l;
  sensor->odometry_count = count_o;

  // printf("%d %d\n", count_l, count_o);
  new_hornetsoft_sensor(sensor, count_l, count_o);

  count_l = -1, count_o = -1;
  rewind(fp);
  while(fscanf(fp,"%s", temp) != -1){
    if(strcmp(temp, "L") == 0){
      state_lo = 1;
      count_l = count_l + 1;
    }else if(strcmp(temp, "O") == 0){
      state_lo = 0;
      count_o = count_o + 1;
    }

    if(state_lo == 1){
      int i = 0;
      for(i = 0; i < 3+3+180+1; i++){
        fscanf(fp,"%f",&ftemp);
        _ftemp[i] = ftemp;
      }
      sensor->laser[count_l].v1.x = _ftemp[0];
      sensor->laser[count_l].v1.y = _ftemp[1];
      sensor->laser[count_l].v1.theta = _ftemp[2];
      sensor->laser[count_l].v2.x = _ftemp[3];
      sensor->laser[count_l].v2.y = _ftemp[4];
      sensor->laser[count_l].v2.theta = _ftemp[5];

      for(i = 6; i < 6+180; i++){
        sensor->laser[count_l].r[i-6] = _ftemp[i];
      }
      sensor->laser[count_l].ts = _ftemp[i];

      // for(i = 0; i < 180; i++){
        // printf("%f\n", sensor->laser->r[i]);
      // }

      // printf("%f, %f,%f\n", sensor->laser->r[0], sensor->laser->v1.x, sensor->laser->ts);

    }else{
      fscanf(fp,"%f %f %f %f", 
        &sensor->odometry[count_o].v.x, &sensor->odometry[count_o].v.y, 
        &sensor->odometry[count_o].v.theta, &sensor->odometry[count_o].ts);

      // printf("%f, %f, %f, %f\n", sensor->odometry[0].v.x, sensor->odometry[0].v.y, 
      //   sensor->odometry[0].v.theta, sensor->odometry[0].ts);
      // printf("%f, %f, %f, %f\n", sensor->odometry[1].v.x, sensor->odometry[1].v.y, 
      //   sensor->odometry[1].v.theta, sensor->odometry[1].ts);
      // printf("%f, %f, %f, %f\n", sensor->odometry[2].v.x, sensor->odometry[2].v.y, 
      //   sensor->odometry[2].v.theta, sensor->odometry[2].ts);
      // printf("%f, %f, %f, %f\n", sensor->odometry[3].v.x, sensor->odometry[3].v.y, 
      //   sensor->odometry[3].v.theta, sensor->odometry[3].ts);
    }
  }
  // printf("%d %d\n", sensor->laser_count, sensor->odometry_count);
  fclose(fp);
  printf("Complete Sensor Usage \n");
  return 0;
}