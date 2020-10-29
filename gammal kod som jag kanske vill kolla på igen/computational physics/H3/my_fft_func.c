#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <gsl/gsl_fft_complex.h>
#include "my_fft_func.h"

/* Performs FFT of data and returns the result in transformed_data */
void fft(double complex *data, double complex *transformed_data, int data_length) {

	/* Declaration of variables */
	int i; // For summing over
	double tmp_array[data_length * 2]; // Temporary array for copying data into data_cp
	int stride = 1; // Separation between consecutive elements in data (argument in gsl_fft_complex_forward)

	gsl_fft_complex_wavetable * wavetable = gsl_fft_complex_wavetable_alloc(data_length);
	gsl_fft_complex_workspace * work = gsl_fft_complex_workspace_alloc(data_length);

	/* Make copy data_cp of type gsl_complex_packed_array of data */
	for (i = 0; i < data_length; i++) {

		tmp_array[2 * i] = creal(data[i]);
		tmp_array[2 * i + 1] = cimag(data[i]);
	}
	gsl_complex_packed_array data_cp = tmp_array; // Copy of data


	/* Perform FFT */
	int tmp = gsl_fft_complex_transform(data_cp,
		stride,
		data_length,
		wavetable,
		work,
		-1);
	tmp++; // This is a bodgy way of suppressing warnings about gsl_fft_complex_forward's return value being unused
	

	/* Save Fourier transformed data in transformed_data */
	for (i = 0; i < data_length; i++) {
		transformed_data[i] = (data_cp[2 * i] + data_cp[2 * i + 1] * I);
	}

	/*Free memory of wavetable and workspace*/
	gsl_fft_complex_wavetable_free(wavetable);
	gsl_fft_complex_workspace_free(work);
}

/* Performs inverse FFT of data and returns the result in transformed_data */
void fft_inv(double complex *data, double complex *transformed_data, int data_length) {

	/* Declaration of variables */
	int i; // For summing over
	double tmp_array[data_length * 2]; // Temporary array for copying data into data_cp
	int stride = 1; // Separation between consecutive elements in data (argument in gsl_fft_complex_forward)

	gsl_fft_complex_wavetable * wavetable = gsl_fft_complex_wavetable_alloc(data_length);
	gsl_fft_complex_workspace * work = gsl_fft_complex_workspace_alloc(data_length);

	/* Make copy data_cp of type gsl_complex_packed_array of data */
	for (i = 0; i < data_length; i++) {

		tmp_array[2 * i] = creal(data[i]);
		tmp_array[2 * i + 1] = cimag(data[i]);
	}
	gsl_complex_packed_array data_cp = tmp_array; // Copy of data


	/* Perform inverse FFT */
	int tmp = gsl_fft_complex_inverse(data_cp,
		stride,
		data_length,
		wavetable,
		work);
	tmp++; // This is a bodgy way of suppressing warnings about gsl_fft_complex_forward's return value being unused
	

	/* Save transformed data in transformed_data */
	for (i = 0; i < data_length; i++) {
		transformed_data[i] = (data_cp[2 * i] + data_cp[2 * i + 1] * I);
	}

	/*Free memory of wavetable and workspace*/
	gsl_fft_complex_wavetable_free(wavetable);
	gsl_fft_complex_workspace_free(work);
}

/* Makes a frequency array fft_freq with frequency interval 1/(dt*n) centered at 0 */
void fft_freq(double *fft_freq, double dt, int n) /* output frequency array, timestep, number of timesteps */
{
	/* Declaration of variables */
    	int i;
	/* Fill the output array with shifted frequencies */
	for (i = 0; i < n; i++)	{
		if (n % 2) { // if n odd
			if (i <= n / 2)	{
				fft_freq[i] = i / (dt * n);
			} else {
				fft_freq[i] = -(n - i) / (dt * n);
			}			
		} else { // if n even
			if (i < n / 2) {
				fft_freq[i] = i / (dt * n);
			} else {
				fft_freq[i] = -(n - i) / (dt * n);
			}			
		}
	}
}


