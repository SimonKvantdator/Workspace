#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[])
{
int *as = NULL;
int sum = 0;

for ( int ix = 0; ix < 10; ++ix )
  as[ix] = ix;

for ( int ix = 0; ix < 10; ++ix )
  sum += as[ix];

free(as);

}
