#include "log.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void new_hornetsoft_log(log_type *log)
{
	log->point = (point_type *)calloc(log->count, sizeof(point_type));
	log->node_id = (long *)calloc(log->count, sizeof(long));
	log->node_label = (int *)calloc(log->count, sizeof(int));
	log->feature = (feature_type *)calloc(log->count, sizeof(feature_type));
}

int read_log(const char *logName, log_type *log)
{
	long count = -1, temp_count;
	point_type temp_point;
	string line;

	ifstream myFile(logName);
	if(myFile.is_open()){
  		while(getline (myFile, line)){
  			if (isdigit(line[0]))
  			{
  				istringstream ss(line);
  				count = count + 1;
  				ss >> temp_point.x >> temp_point.y >> temp_point.z;
  				ss >> temp_count;
  			}
  		}
  		myFile.close();
  		cout << "Number of Data " << count << ' ' << temp_count << '\n';
  		log->count = count + 1;
  		count = -1;
  		new_hornetsoft_log(log);
  		myFile.open(logName);
  		if (myFile.is_open())
  		{
  			while(getline (myFile, line)){
  				if (isdigit(line[0]))
  				{
  					istringstream ss(line);
  					count = count + 1;
  					ss >> log->point[count].x >> log->point[count].y >> log->point[count].z;
  					ss >> log->node_id[count] >> log->node_label[count];
  					for(int i = 0; i < 9; i++){
  						ss >> log->feature[count].f[i];
  					}
  					ss >> log->feature[count].bias;
  				}
  			}
  			myFile.close();
  		}else{
  			fprintf(stderr, "# Could not open file %s\n", logName);
    	return -1;
  		}
  	}else{
  		fprintf(stderr, "# Could not open file %s\n", logName);
    	return -1;
  	}
  return 0;
}

void sort_log(log_type log, log_type *log_sort){
  long node_veg[log.count] = {0}, node_wire[log.count] = {0}, node_pole[log.count] = {0}, node_ground[log.count] = {0}, node_facade[log.count] = {0};
  long count = 0, k;
  for(int i = 0; i < log.count; i++){
    if(log.node_label[i] == VEG){
      node_veg[i] = log.node_id[i] + 1;
    }else if(log.node_label[i] == WIRE){
      node_wire[i] = log.node_id[i] + 1;
    }else if(log.node_label[i] == POLE){
      node_pole[i] = log.node_id[i] + 1;
    }else if(log.node_label[i] == GROUND){
      node_ground[i] = log.node_id[i] + 1;
    }else if(log.node_label[i] == FACADE){
      node_facade[i] = log.node_id[i] + 1;
    }
  }

  log_sort->count = log.count;
  new_hornetsoft_log(log_sort);
  for(int i = 0; i < log.count; i++){
    if(node_veg[i] != 0){
      k = node_veg[i] - 1;
      log_sort->node_id[count] = count;
      log_sort->node_label[count] = VEG;
      log_sort->point[count] = log.point[k];
      log_sort->feature[count] = log.feature[k];
      count = count + 1;
    }
  }
  for(int i = 0; i < log.count; i++){
    if(node_wire[i] != 0){
      k = node_wire[i] - 1;
      log_sort->node_id[count] = count;
      log_sort->node_label[count] = WIRE;
      log_sort->point[count] = log.point[k];
      log_sort->feature[count] = log.feature[k];
      count = count + 1;
    }
  }
  for(int i = 0; i < log.count; i++){
    if(node_pole[i] != 0){
      k = node_pole[i] - 1;
      log_sort->node_id[count] = count;
      log_sort->node_label[count] = POLE;
      log_sort->point[count] = log.point[k];
      log_sort->feature[count] = log.feature[k];
      count = count + 1;
    }
  }
  for(int i = 0; i < log.count; i++){
    if(node_ground[i] != 0){
      k = node_ground[i] - 1;
      log_sort->node_id[count] = count;
      log_sort->node_label[count] = GROUND;
      log_sort->point[count] = log.point[k];
      log_sort->feature[count] = log.feature[k];
      count = count + 1;
    }
  }
  for(int i = 0; i < log.count; i++){
    if(node_facade[i] != 0){
      k = node_facade[i] - 1;
      log_sort->node_id[count] = count;
      log_sort->node_label[count] = FACADE;
      log_sort->point[count] = log.point[k];
      log_sort->feature[count] = log.feature[k];
      count = count + 1;
    }
  }
}