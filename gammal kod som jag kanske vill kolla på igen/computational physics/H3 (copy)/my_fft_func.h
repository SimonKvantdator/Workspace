#include <complex.h>
void fft(double complex *data, double complex *transformed_data, int data_length);
void fft_inv(double complex *data, double complex *transformed_data, int data_length);
void fft_freq(double *fft_freq, double dt, int n);