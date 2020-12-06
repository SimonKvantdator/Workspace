
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

const int size = 30000;


void
mul_cpx(
    double *c_re,
    double *c_im,
    double *a_re,
    double *a_im,
    double *b_re,
    double *b_im
    );

int main(int argc, char *argv[])
{
    // Declare arrays
    //Do we need to make these variables volatile?
    //or will they be optimized away with O2?
    double *as_re, *as_im, *bs_re, *bs_im, *cs_re, *cs_im;

    // Initialize arrays
    as_re = (double *)malloc(sizeof(double) * size);
    as_im = (double *)malloc(sizeof(double) * size);
    bs_re = (double *)malloc(sizeof(double) * size);
    bs_im = (double *)malloc(sizeof(double) * size);
    cs_re = (double *)malloc(sizeof(double) * size);
    cs_im = (double *)malloc(sizeof(double) * size);

    for (size_t i = 0; i < size; ++i)
    {
        bs_re[i] = 1;
        bs_im[i] = 1;
        cs_re[i] = 1;
        cs_im[i] = 1;
    }
    struct timespec ts1, ts2;
    timespec_get(&ts1, TIME_UTC);
    for (size_t ix; ix < 200000; ix++) {
        for (size_t i = 0; i < size; ++i){
            mul_cpx(&cs_re[i], &cs_im[i],
                &as_re[i], &as_im[i], &bs_re[i], &bs_im[i]); }
    }


        // End timing
    timespec_get(&ts2, TIME_UTC);
    double diff = (ts2.tv_sec + ts2.tv_nsec * 1.0e-9) - (ts1.tv_sec + ts1.tv_nsec * 1.0e-9);
    
    printf("Time milliseconds: %f\n", diff * 1.e3/200000);
    printf("First element of array: %f\n", as_im[1]);

    free(as_re); free(as_im); free(bs_re); free(bs_im); free(cs_re); free(cs_im);
}


void
mul_cpx(
    double *c_re,
    double *c_im,
    double *a_re,
    double *a_im,
    double *b_re,
    double *b_im
    ){
*a_re = (*b_re) * (*c_re) - (*b_im) * (*c_im);
*a_im = (*b_re) * (*c_im) + (*b_im) * (*c_re);
}
