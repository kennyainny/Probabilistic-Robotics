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
