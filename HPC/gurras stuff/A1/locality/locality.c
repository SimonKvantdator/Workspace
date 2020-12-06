#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void
row_sums(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
);

void
col_sums(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
  );

void
col_sums2(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
  );
 
int main(int argc, char *argv[]){
  int nrs = 1000; 
  int ncs = 1000;
  srand (time ( NULL));
  // would have to be the largest of nrs and ncs in the malloc
  double *sums = (double*)malloc(sizeof(double)*nrs);
  double *elements = (double*)malloc(sizeof(double)*nrs*ncs);
  double **matrix = (double**)malloc(sizeof(double*)*nrs);

  for(size_t ix = 0, jx = 0; ix < nrs; ++ix, jx += ncs)
      matrix[ix] = elements + jx;

  for ( size_t ix = 0; ix < nrs; ++ix ) {
      for ( size_t jx = 0; jx < ncs; ++jx )
          matrix[ix][jx] = (double)rand()/RAND_MAX*2.0;
  }
  const double **cmatrix = (const double**)matrix;
  
  //start timer
  struct timespec ts1, ts2;
  timespec_get(&ts1, TIME_UTC);

  int runs = 5000;
  for (size_t ix = 0; ix < runs; ix++) {
    row_sums(sums, cmatrix, nrs, ncs);
  }
  //end timer and calc diff
  timespec_get(&ts2, TIME_UTC);
  double diff = (ts2.tv_sec + ts2.tv_nsec * 1.0e-9) - (ts1.tv_sec + ts1.tv_nsec * 1.0e-9);
   
  printf("ms elapsed: %f\n",diff * 1.e3);
  printf("Rowsum 10: %f\n",sums[9]);
    

  timespec_get(&ts1, TIME_UTC);
  for (size_t ix = 0; ix < runs; ix++) {
    col_sums2(sums, cmatrix, nrs, ncs);
  }
  timespec_get(&ts2, TIME_UTC);
  diff = (ts2.tv_sec + ts2.tv_nsec * 1.0e-9) - (ts1.tv_sec + ts1.tv_nsec * 1.0e-9);
   
  printf("ms elapsed: %f\n",diff * 1.e3);
  printf("Colsum 10: %f\n",sums[9]);
  free(sums);
  free(matrix);
  free(sums);
}




void
row_sums(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
)
{
  for ( size_t ix = 0; ix < nrs; ++ix ) {
    double sum = 0;
    for ( size_t jx = 0; jx < ncs; ++jx )
      sum += matrix[ix][jx];
    sums[ix] = sum;
  }
}

void
col_sums(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
  )
{
  for ( size_t jx = 0; jx < ncs; ++jx ) {
    double sum = 0;
    for ( size_t ix = 0; ix < nrs; ++ix )
      sum += matrix[ix][jx];
    sums[jx] = sum;
  }
}
 void
col_sums2(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
  )
{
  for ( size_t ix=0; ix < nrs; ++ix ) {
    for ( size_t jx=0; jx < ncs; ++jx )
      sums[jx] += matrix[ix][jx];
  }
}
