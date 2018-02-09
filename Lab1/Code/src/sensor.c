#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float z_ks(map_type map, state_type state){

  int x_test,y_test,x,y;
  float r = 0.1, z_kp;
  x = (int)state.x;
  y = (int)state.y;
  // printf("Intial position \n");
  // printf("%i\n",x);
  // printf("%i\n",y);
  // printf("%f\n",p_state.theta);
  // printf("Probability\n");
  // printf("%f\n",map.prob[x][y]);
  while (r <= MAX_LASER){
    x_test = round(state.x + r*cos(state.theta));
    y_test = round(state.y + r*sin(state.theta));
  // printf("**********Point*******\n");
  // printf("%d\n",x_test);
  // printf("%d\n",y_test);
  // printf("%f\n",map.prob[x_test][y_test]);
    if (map.prob[x_test][y_test] == 1){
    // printf("Filled X and Y \n");
    // printf("%i\n",x_test);
    // printf("%i\n",y_test);
    // printf("Filled Probability \n");
    // printf("%f\n",map.prob[x_test][y_test]);
      printf("zk*\n");
      z_kp = sqrt(pow((x-x_test),2) + pow((y-y_test),2));
      printf("%f\n",z_kp);
      break;
    }
    r = r + 0.5;
  }
  printf("break \n");
  return z_kp;
}

float sensor_model(laser_type laser, state_type state, map_type map, intrinsic_param_type param){
  // for each given xi, z* will be changed
  printf("aaa");
  float q = 1, p, z;

  int zks, range = 1;  

  float z_hit, z_short, z_max, z_rand, sig_hit, lamb_short;
  float p_hit, p_short, p_max, p_rand;

  for(int i = 0; i < 180; i++){
    zks = (int)z_ks(map, state);    

    // assume for now
    // zks = 0;
    param.z_hit[zks] = 0.25;
    param.z_short[zks] = 0.25;
    param.z_max[zks] = 0.25;
    param.z_rand[zks] = 0.25;
    param.lamb_short[zks] = 1;
    param.sig_hit[zks] = 1;

    z = laser.r[i];

    z_hit = param.z_hit[zks];
    z_short = param.z_short[zks];
    z_max = param.z_max[zks];
    z_rand = param.z_rand[zks];
    lamb_short = param.lamb_short[zks];
    sig_hit = param.sig_hit[zks];

    p_hit = normal_dist(z, MAX_LASER, zks, sig_hit);
    p_short = exp_dist(z, zks, lamb_short);
    p_max = narrow_uniform_dist(z, MAX_LASER, range);
    p_rand = uniform_dist(z, MAX_LASER);

    p = z_hit*p_hit + z_short*p_short + z_max*p_max + z_rand*p_rand;
    q = q*p;
  }
  return pow(q,1/180);
}

void intrinsic_parameters(state_type p_state, map_type map, sensor_type sensor, intrinsic_param_type *param){
  // float omicron[6];
  // float eta,e_hit,e_short,e_max,e_rand,z_hit,z_short,z_max,z_rand,z_sum,big_z;
  // int j, k=0,i;
  // z_ks(map,p_state);
  // for (i=0;i<=180*sensor.laser_count;i=i+1){
  // eta=normal_dist()+exp_dist()+narrow_uniform_dist()+uniform_dist();
  // e_hit[i]=eta*norm_dist();
  // e_short[i]=eta*exp_dist();
  // e_max[i]=eta*narrow_uniform_dist();
  // e_rand[i]=eta*uniform_dist();
  // z_sum=pow(sensor.laser[j].r[k],2)+sum;
  // k=k+1;
  // if (k>=180){
  //   k=0;
  //   j=j+1;
  // }

  // }
  // big_z=sqrt(z_sum);
  // z_hit=1

  // printf("Big Z\n");
  // printf("%i\n",big_z);
  int z_s = 0;
  param->z_hit[z_s] = 0;
}

void intrinsic_parameters_initialize(intrinsic_param_type *param){
  for(int i = 0; i < MAX_LASER; i++){
    param->z_hit[i] = 0.25;
    param->z_short[i] = 0.25;
    param->z_max[i] = 0.25;
    param->z_rand[i] = 0.25;
    param->sig_hit[i] = 1.0;
    param->lamb_short[i] = 0.5;
  }
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
  printf("Complete Reading Sensor\n");
  return 0;
}