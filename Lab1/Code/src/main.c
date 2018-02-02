#include "main.h"
#include <stdio.h>
int main(int argc, char *argv[]) //char **argv[]
{
   printf("Hello, World!");

   map_type map;
   read_beesoft_map(argv[1], &map);

   // int i = 1, j = 1;

   for(int i = 1; i <= map.size_x; i++){
   	for(int j = 1; j <= map.size_y; j++){
   		printf("%.2f ",map.prob[i][j]);
   	}
   	printf(" \n");
   }
   
   return 0;
}