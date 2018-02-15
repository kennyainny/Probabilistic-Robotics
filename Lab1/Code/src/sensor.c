#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float z_ks(map_type map, state_type state){
  // printf("ccc");
  int x_test,y_test,x,y;
  float r = 0.1, z_kp = 1.0;
  x = (int)state.x;
  y = (int)state.y;
  // printf("Intial position \n");
  // printf("%i\n",x);
  // printf("%i\n",y);
  // printf("%f\n",p_state.theta);
  // printf("Probability\n");
  // printf("%f\n",map.prob[x][y]);

  while (r <= MAX_LASER/10){
  //   // printf("deg %f\n", DEG(state.theta));
    x_test = round(state.x + r*cos(state.theta));
    y_test = round(state.y + r*sin(state.theta));
  //   // printf("**********Point*******\n");
    // printf("x_test %d\n",x_test);
  //   printf("y_test %d\n",y_test);
  //   // printf("prob %f\n",map.prob[x_test][y_test]);

    if(x_test >= map.size_x || y_test >= map.size_y || x_test < 0 || y_test < 0){
      z_kp = MAX_LASER/10;
      break;
    }else if (map.prob[x_test][y_test] == 1){
      // printf("Filled X and Y \n");
      // printf("%d %d %d %d\n",x_test, x, y_test, y);
      // printf("Filled Probability \n");
      // printf("%f\n",map.prob[x_test][y_test]);
      // printf("x-x_test: %d\n",(x-x_test));
      z_kp = sqrt(pow((x-x_test),2) + pow((y-y_test),2));
      // printf("zk*: %f\n",z_kp);
      break;
    }
    r = r + 0.5;//0.5;
    // printf("%f\n", r);
  }
  // printf("%f\n", r);
  // printf("break \n");
  // if(z_kp == 0){
  //   printf("x_test %d\n",x_test);
  // printf("y_test %d\n",y_test);
  // printf("x %d\n",x);
  // printf("y %d\n",y);
  // printf("theta %f\n",state.theta);
  // printf("prob %f\n",map.prob[x_test][y_test]);
  // }
  // printf("zk*: %f\n",z_kp);
  return z_kp;//*10.0;
}

float sensor_model(laser_type laser, state_type state, map_type map, intrinsic_param_type param){
  // for each given xi, z* will be changed
  // printf("aaa");
  float q = 1, p, z, log_sum = 0;

  float zks, range = 0.05*MAX_LASER;  

  int zk_s;

  float z_hit, z_short, z_max, z_rand, sig_hit, lamb_short;
  float p_hit, p_short, p_max, p_rand;

  state_type temp_state = state;

  for(int i = 45; i < 135; i = i + 2){
    // int i = (int)round(state.theta);
    if(state.theta >= 0 && state.theta <= M_PI){
      temp_state.theta = RAD(i);
    }else{
      temp_state.theta = RAD((i-180));
    }
    
    zks = z_ks(map, temp_state)*10.0; 
    // printf("abcd\n");   
    // printf("ddd zk*: %f %f %i\n",DEG(temp_state.theta), temp_state.theta, zks);
    
    if(zks > 0){
      zk_s = (int)round(zks)-1;
      // printf("%d %f\n", zk_s,zks);
    
      z = laser.r[i];

      // param.zhit[zk_s] = 0.7;
      // param.zshort[zk_s] = 0.1;
      // param.zmax[zk_s] = 0.1;

      // printf("aa %d\n", zk_s);
      // // param.zrand[zk_s] = 0.1;
      // param.lambshort[zk_s] = 0.05;
      // param.sighit[zk_s] = 500;

      // z_hit = param.z_hit[zk_s];
      // z_short = param.z_short[zk_s];
      // z_max = param.z_max[zk_s];
      // z_rand = param.z_rand[zk_s];
      // lamb_short = param.lamb_short[zk_s];
      // sig_hit = param.sig_hit[zk_s];

      z_hit = 0.25;
      z_short = 0.15;
      z_max = 0.15;
      z_rand = 0.15;
      lamb_short = 0.01;
      sig_hit = 200;

      p_hit = normal_dist(z, MAX_LASER, zks, sig_hit);
      p_short = exp_dist(z, zks, lamb_short);
      p_max = narrow_uniform_dist(z, MAX_LASER, range);
      p_rand = uniform_dist(z, MAX_LASER);

      p = z_hit*p_hit + z_short*p_short + z_max*p_max + z_rand*p_rand;
    }

    if(p > 0.0){
      log_sum = log_sum + log(p);
    }
    // printf("ddd\n");
  //   if(isnan(p_hit)){
  //   printf("%d %f %d %f %f\n",i, z, zks, p, log_sum);
  //   printf("p: %d %f %f %f %f\n",i, p_hit, p_short, p_max, p_rand);
  //   printf("z %f %f %f %f %f %f\n",z_hit, z_short, z_max, z_rand, lamb_short, sig_hit);
  // }
  }
  // while(1);
  // printf("%f\n", log_sum);
  // printf("eee\n");
  log_sum = log_sum/180;
  q = exp(log_sum);
  // printf("%f %f\n", log_sum, q);
  return q;
}

float sensor_model2(laser_type laser, state_type state, map_type map, intrinsic_param_type param){
  float p = 0.0;
  float angle, x_ray_casting, y_ray_casting;
  state_type laser_state; //25 cm ofset
  laser_state.theta = state.theta;
  laser_state.x = state.x + (25/map.resolution)*cos(laser_state.theta);
  laser_state.y = state.y + (25/map.resolution)*sin(laser_state.theta);

  if(laser_state.x <= map.size_x && laser_state.y <= map.size_y && laser_state.x > 0 && laser_state.y > 0){ //inside the map
    if(map.prob[(int)laser_state.x][(int)laser_state.y] >= 0 && map.prob[(int)laser_state.x][(int)laser_state.y] < 1){

      for (int i = 0; i < MAX_LASER; i++){
        angle = RAD((float)i) + laser_state.theta;
        x_ray_casting = laser_state.x + laser.r[i] * cos(angle - M_PI / 2);
        y_ray_casting = laser_state.y + laser.r[i] * sin(angle - M_PI / 2);

        if(x_ray_casting <= map.size_x && y_ray_casting <= map.size_y && x_ray_casting > 0 && y_ray_casting > 0){ //inside the map
          if(map.prob[(int)x_ray_casting][(int)y_ray_casting] >= 0 && map.prob[(int)x_ray_casting][(int)y_ray_casting] < 1){
            p = p + 1;
          }
        }
      }
    }
  }
  return p/180.0;
}

// void calc_z_star_array(particle_type particle, float **z, map_type map){
//   int i;
//   z = (float *)calloc(particle.particle_count, sizeof(float));
//   for(i=0;i<=particle.particle_count;i=i+1){
//     *z[i]=z_ks(map, particle.state[i]);
//     printf("%f\n",z[i]);

//   }
// }

void intrinsic_parameters(state_type p_state, map_type map, sensor_type sensor, intrinsic_param_type *param,particle_type particle){
  int size_z = particle.particle_count; //sizeof(z)/sizeof(z[0]);
  float omicron[6],e_hit[size_z],e_short[size_z],e_max[size_z],e_rand[size_z];
  float eta,z_hit,z_short,z_max,z_rand,z_sum=0,big_z,sig,lambda,e_hit_sum=0,e_short_sum=0,e_max_sum=0,e_rand_sum=0,sum_ehit_zzs=0,sum_eshort_z=0,trial;
  lambda=0.01,sig=0.5;
  int j=0, k=0,i,c;

  //This works
  // z = (float *)calloc(particle.particle_count, sizeof(float));
  float z[particle.particle_count];
  for(i=0;i<=size_z;i=i+1){
    z[i]=z_ks(map, particle.state[i]);
    printf("z*\n");
    printf("%i\n",i);

  }
  c=0;
  // for(c=0;c<=size_z;c=c+1){
    for (i=0;i<=size_z;i=i+1){
      // printf("%f\n",normal_dist(sensor.laser[j].r[k],9000,z[i],sig));
      // printf("z*\n");
      // printf("%f\n",z[c]);
      // printf("Laser\n");
      // printf("%f\n",sensor.laser[j].r[k]);
      eta=1/(normal_dist(sensor.laser[j].r[k],9000,z[c],sig)+exp_dist(sensor.laser[i].r[i],z[c],lambda)+narrow_uniform_dist(sensor.laser[i].r[i],9000,z[c])+uniform_dist(sensor.laser[i].r[i],9000));
      // printf("eta\n");
      e_hit[i]=eta*normal_dist(sensor.laser[j].r[k],9000,z[c],sig);
      e_short[i]=eta*exp_dist(sensor.laser[j].r[k],z[c],lambda);
      e_max[i]=eta*narrow_uniform_dist(sensor.laser[j].r[k],9000,z[c]);
      e_rand[i]=eta*uniform_dist(sensor.laser[j].r[k],9000);
      z_sum=pow(sensor.laser[j].r[k],2)+z_sum;
      e_hit_sum=e_hit[i]+e_hit_sum;
      e_short_sum=e_short[i]+e_short_sum;
      e_max_sum=e_max[i]+e_max_sum;
      e_rand_sum=e_rand[i]+e_rand_sum;
      sum_ehit_zzs=(e_hit[i]*pow((sensor.laser[j].r[k]-z[c]),2))+sum_ehit_zzs;
      sum_eshort_z=e_short[i]*sensor.laser[j].r[k]+sum_eshort_z;
      // printf("p_hit\n");
      // printf("%f\n",normal_dist(sensor.laser[j].r[k],9000,z[c],sig));
      // printf("p_short\n");
      // printf("%f\n",exp_dist(sensor.laser[j].r[k],z[c],lambda));
      // printf("p_rand\n");
      // printf("%f\n",uniform_dist(sensor.laser[j].r[k],9000));
      // printf("\n");
      // printf("%f\n",sensor.laser[j].r[k]);
      // printf("%f\n",z[i]);
      k=k+1;
        if (k>=180){
          k=0;
          j=j+1;
        }

    }
    // printf("z_sum\n");
    // printf("%f\n",z_sum);
    // printf("e_sum values\n");
    // printf("%f\n",e_hit_sum);
    // printf("%f\n",e_short_sum);
    // printf("%f\n",e_max_sum);
    // printf("%f\n",e_rand_sum);

    // printf("e_sum_z values\n");
    // printf("%f\n",sum_eshort_z);
    
    big_z=sqrt(z_sum);
    // printf("big_z\n");
    // printf("%f\n",big_z);
    // printf("e_hit_sum\n");
    // printf("%f\n",e_hit_sum);
    // printf("e_short_sum\n")(
    // printf("%f\n",e_short_sum);
    z_hit=(1/big_z)*e_hit_sum;
    z_short=(1/big_z)*e_short_sum;
    z_max=(1/big_z)*e_max_sum;
    z_rand=(1/big_z)*e_rand_sum;
    printf("z parameters\n");
    printf("%f\n",z_hit);
    printf("%f\n",z_short);
    printf("%f\n",z_max);
    printf("%f\n",z_rand);



    sig=sqrt((1/sqrt(e_hit_sum))*sum_ehit_zzs); //how do i access z?
    lambda=(e_short_sum/sum_eshort_z);
    printf("Lambda\n");
    printf("%f\n",lambda);

    printf("Sigma\n");
    printf("%f\n",sig);
  // }
  int z_s = 0;
  // param->z_hit[z_s] = 0;
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