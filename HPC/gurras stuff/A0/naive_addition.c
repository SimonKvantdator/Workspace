#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[])
{
  const int sz = 1<<30;
  long long int badboy = 0;


  clock_t start, end;
  double cpu_time_used;
  start = clock(); 
  for(int ix = 0; ix < sz; ix++)
    badboy = badboy + ix;
  end = clock();

  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Calculated the sum of the first %i integers\n", sz);
  printf("Result was %lld, and it took %f seconds.\n",badboy, cpu_time_used);
  
}
