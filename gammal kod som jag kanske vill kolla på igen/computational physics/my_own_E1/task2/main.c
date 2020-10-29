#include <stdio.h> // for printf
#include <stdlib.h> //for dynamic memory allocation
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <complex.h>
#include "functions.h"

int N = 250;
double dt = 0.1;

double f = 2.0;
double a = 1.0;
double phi = 0.0;

void main()
{
    /*initiate list of times from 0 to N * dt */
    double *tList = malloc(N * sizeof(double));
    double *hList = malloc(N * sizeof(double));
    double t;
    int i;
    for (i=0; i < N; i++)
    {
        t = dt * i;
        tList[i] = t;
        hList[i] = h(t, f, a, phi);
    }

    printf("%.2f \n", tList[0]);

    /* write function output to file */
    // FILE *function_output = fopen("function_output.bin", "w");
    // FILE *time = fopen("time.bin", "w");
    // FILE *parameters = fopen("parameters.bin", "w");

    // fwrite(tList, sizeof(double), N, time);
    // fwrite(hList, sizeof(double), N, function_output);
    // double paramList[5] = {f, a, phi, N * 1.0, dt};
    // fwrite(paramList, sizeof(double), 5, parameters);

    // fclose(function_output);
    // fclose(time);
    // fclose(parameters);

}






