#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]){
    size_t size = 1000000;
    size_t a = 2;
    size_t p[size];
    size_t runs = 1000;
    srand (time ( NULL));
    double *x = (double*)malloc(sizeof(double)*size);
    double *y = (double*)malloc(sizeof(double)*size);
    //init p linearly and x and y
    for ( size_t ix = 0; ix < size; ++ix  ){
        p[ix] = ix;
        x[ix] = (double)rand()/RAND_MAX*2.0;
        y[ix] = 0;
    }

    struct timespec ts1, ts2;
    timespec_get(&ts1, TIME_UTC);
    // add x to y linearly
    for(size_t ix = 0; ix < runs; ix++){
        for ( size_t kx = 0; kx < size; ++kx ) {
            //size_t jx = p[kx];
            y[kx] += a * x[kx];
        }
    }
    timespec_get(&ts2, TIME_UTC);
    double diff = (ts2.tv_sec + ts2.tv_nsec * 1.0e-9) - (ts1.tv_sec + ts1.tv_nsec * 1.0e-9);
    printf("ms elapsed for linear: %f\n",diff * 1.e3);
    printf("x[100]: %f   y[100]: %f\n", x[100], y[100]);


    // p with jumps
    size_t size_jump = 1000;
        for ( size_t jx = 0, kx = 0; jx < size_jump; ++jx)
            for ( size_t ix = jx; ix < size; ix += size_jump, ++kx)
                p[ix] = kx;

    //add x and y jumpy
    timespec_get(&ts1, TIME_UTC);
    for(size_t ix = 0; ix < runs; ix++){
        for ( size_t kx = 0; kx < size; ++kx ) {
            size_t jx = p[kx];
            y[jx] += a * x[jx];
        }
    } 
    timespec_get(&ts2, TIME_UTC);
    diff = (ts2.tv_sec + ts2.tv_nsec * 1.0e-9) - (ts1.tv_sec + ts1.tv_nsec * 1.0e-9);
    printf("ms elapsed for jumpy: %f\n",diff * 1.e3);
    printf("x[100]: %f   y[100]: %f\n", x[100], y[100]);
}
