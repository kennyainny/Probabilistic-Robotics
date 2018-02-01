#include "main.h"
#include <stdio.h>
int main(int argc, char **argv) //char *argv[])
{
   printf("Hello, World!");

   map_type map;
   read_beesoft_map(argv[1], &map);

   printf("%f\n", map.prob[1][1]);   
   return 0;
}