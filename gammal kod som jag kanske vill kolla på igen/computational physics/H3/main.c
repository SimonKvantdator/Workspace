#include <gsl/gsl_rng.h> // For random numbers
#include <complex.h> // Standard Library of complex numbers
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h> // For running loops in parallell
#include "utility_funcs.h"
#include "my_fft_func.h" // Fast Fourier transform

#define PI (3.1415926535897932)
#define hbar (0.658212) // [eV fs]
#define hydrogen_mass (104.5) // [eV fs^2 / A^2]

void task1() {
	/* Declare and initiate some variables */
	int x_index; // For looping over

	double d = 0.5; // wavepacket width [A]
	double x; // Wavefunction's position coordinate [A]
	double x0 = 0.0; // Mean position [A]
	double x_max = 30.0; // x goes from x0 - x_max to x0 + x_max [A]

	int x_list_length = ipow(2, 12); // List length 2^n - 1 optimal for FFT
	double x_list[x_list_length]; // List of positions
	double complex psi_x_list[x_list_length]; // psi(x)
	double real_psi_x_list[x_list_length]; // Real part of psi(x)
	double imag_psi_x_list[x_list_length]; // Imaginary part of psi(x)
	double dx = 2 * x_max / x_list_length;

	double p0 = sqrt(2 * hydrogen_mass * 0.1); // [eV fs / A] mean momentum obtained from solving p0^2 / 2m = 0.1 eV
	double p_list[x_list_length]; // List of momenta
	double complex psi_p_list[x_list_length]; // psi(p)
	double real_psi_p_list[x_list_length]; // Real part of psi(p)
	double imag_psi_p_list[x_list_length]; // Imaginary part of psi(p)
	double normalization; // For normalizing psi(p)

	FILE *x_list_file;
	FILE *real_psi_x_list_file;
	FILE *imag_psi_x_list_file;
	
	FILE *p_list_file;
	FILE *real_psi_p_list_file;
	FILE *imag_psi_p_list_file;


	/* Calculate wavefunction in position basis */
	for(x_index = 0; x_index < x_list_length; x_index++) {

		x = x0 + x_max * 2 * (x_index - x_list_length / 2.0) / x_list_length;
		x_list[x_index] = x;
		psi_x_list[x_index] = psi_x(x, x0, p0, d);
		real_psi_x_list[x_index] = creal(psi_x(x, x0, p0, d));
		imag_psi_x_list[x_index] = cimag(psi_x(x, x0, p0, d));
	}

	/* Calculate wavefunction in momentum basis by using FFT */
	fft(psi_x_list, psi_p_list, x_list_length);
	fft_freq(p_list, dx, x_list_length);
	for(x_index = 0; x_index < x_list_length; x_index++) { // Scale p to have the right unit
		p_list[x_index] *= 2 * PI * hbar;
	}

	/* Normalize and save real and imaginary parts of psi(p) separately for writing to file */
	normalization = 2 * x_max / (x_list_length * sqrt(2 * PI * hbar));
	for(x_index = 0; x_index < x_list_length; x_index++) {
		real_psi_p_list[x_index] = creal(psi_p_list[x_index]) * normalization;
		imag_psi_p_list[x_index] = cimag(psi_p_list[x_index]) * normalization;
	}


	/* Write lists to file */
	x_list_file = fopen("data/x_list_task1.bin", "w");
	real_psi_x_list_file = fopen("data/real_psi_x_list_task1.bin", "w");
	imag_psi_x_list_file = fopen("data/imag_psi_x_list_task1.bin", "w");

	p_list_file = fopen("data/p_list_task1.bin", "w");
	real_psi_p_list_file = fopen("data/real_psi_p_list_task1.bin", "w");
	imag_psi_p_list_file = fopen("data/imag_psi_p_list_task1.bin", "w");

	fwrite(x_list, x_list_length * sizeof(double), 1, x_list_file);
	fwrite(real_psi_x_list, x_list_length * sizeof(double), 1, real_psi_x_list_file);
	fwrite(imag_psi_x_list, x_list_length * sizeof(double), 1, imag_psi_x_list_file);

	fwrite(p_list, x_list_length * sizeof(double), 1, p_list_file);
	fwrite(real_psi_p_list, x_list_length * sizeof(double), 1, real_psi_p_list_file);
	fwrite(imag_psi_p_list, x_list_length * sizeof(double), 1, imag_psi_p_list_file);

	fclose(x_list_file);
	fclose(real_psi_x_list_file);
	fclose(imag_psi_x_list_file);

	fclose(p_list_file);
	fclose(real_psi_p_list_file);
	fclose(imag_psi_p_list_file);
}

void task2() {
	/* Declare and initiate some variables */
	int x_index, time_index;  // For looping over

	double timestep = 0.1; // [fs]
	int nbr_timesteps = 4000;

	double d = 0.5; // wavepacket width [A]
	double x; // Wavefunction's position coordinate [A]
	double x0 = 0.0; // Mean position [A]
	double x_max = 40.0; // x goes from x0 - x_max to x0 + x_max [A]

	int x_list_length = ipow(2, 14); // List length 2^n - 1 optimal for FFT
	double x_list[x_list_length]; // List of positions [A]
	double V_list[x_list_length]; // List of potential energies [eV]
	double complex psi_x_list[x_list_length]; // psi(x) [A^(-1/2)]
	double *real_psi_x_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Real part of psi(x), real_psi_x_list[x_index + x_list_length * time_index], [A^(-1/2)]
	double *imag_psi_x_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Imaginary part of psi(x), imag_psi_x_list[x_index + x_list_length * time_index], [A^(-1/2)]
	double dx = 2 * x_max / x_list_length;
	double normalization = 2 * x_max / (x_list_length * sqrt(2 * PI * hbar)); // For normalizing psi(p)

	double p0 = sqrt(2 * hydrogen_mass * 0.1); // [eV fs / A] mean momentum obtained from solving p0^2 / 2m = 0.1 eV
	double p_list[x_list_length]; // List of momenta [eV fs / A]
	double complex psi_p_list[x_list_length]; // psi(p) [(eV fs / A)^(-1/2)]
	double *real_psi_p_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Real part of psi(p), real_psi_p_list[x_index + x_list_length * time_index], [(eV fs / A)^(-1/2)]
	double *imag_psi_p_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Imaginary part of psi(p), imag_psi_x_list[x_index + x_list_length * time_index], [(eV fs / A)^(-1/2)]

	int nbr_parameters = 7;
	double parameters[] = {x0, x_max, x_list_length, timestep, nbr_timesteps, p0, d};

	FILE *x_list_file;
	FILE *real_psi_x_list_file;
	FILE *imag_psi_x_list_file;
	
	FILE *p_list_file;
	FILE *real_psi_p_list_file;
	FILE *imag_psi_p_list_file;

	FILE *parameters_file;


	/* Calculate wavefunction in position basis at time 0 */
	for(x_index = 0; x_index < x_list_length; x_index++) {
		x = x0 + x_max * 2 * (x_index - x_list_length / 2.0) / x_list_length;
		x_list[x_index] = x;

		psi_x_list[x_index] = psi_x(x, x0, p0, d);
		// if (cabs(psi_x(x, x0, p0, d)) > 0.0001) {
		// 	printf("psi_x(x, x0, p0, d) = %f + %fi\n", creal(psi_x(x, x0, p0, d)), cimag(psi_x(x, x0, p0, d))); // BODGE
		// }
		real_psi_x_list[x_index + x_list_length * 0] = creal(psi_x(x, x0, p0, d));
		imag_psi_x_list[x_index + x_list_length * 0] = cimag(psi_x(x, x0, p0, d));

		V_list[x_index] = V_task2(x);
	}

	/* Calculate wavefunction in momentum basis at time 0 */
	fft(psi_x_list, psi_p_list, x_list_length);
	fft_freq(p_list, dx, x_list_length);
	for(x_index = 0; x_index < x_list_length; x_index++) {
		p_list[x_index] *= 2 * PI * hbar; // Scale p to have the right unit

		real_psi_p_list[x_index + x_list_length * 0] = creal(psi_p_list[x_index]) * normalization;
		imag_psi_p_list[x_index + x_list_length * 0] = cimag(psi_p_list[x_index]) * normalization;
	}


	/* Time evolve the state */
	for (time_index = 1; time_index < nbr_timesteps; time_index++) {

		/* Perform a single timestep */
		perform_timestep_task2(psi_x_list, x_list, x_list_length, dx, V_list, timestep);

		/* Calculate wavefunction in momentum basis by using FFT */
		fft(psi_x_list, psi_p_list, x_list_length);

		/* Save real and imaginary parts of wavefunction separately for writing to file */
		for(x_index = 0; x_index < x_list_length; x_index++) {
			real_psi_x_list[x_index + x_list_length * time_index] = creal(psi_x_list[x_index]);
			imag_psi_x_list[x_index + x_list_length * time_index] = cimag(psi_x_list[x_index]);
			real_psi_p_list[x_index + x_list_length * time_index] = creal(psi_p_list[x_index]) * normalization;
			imag_psi_p_list[x_index + x_list_length * time_index] = cimag(psi_p_list[x_index]) * normalization;
		}
	}


	/* Write lists to file */
	x_list_file = fopen("data/x_list_task2.bin", "w");
	real_psi_x_list_file = fopen("data/real_psi_x_list_task2.bin", "w");
	imag_psi_x_list_file = fopen("data/imag_psi_x_list_task2.bin", "w");

	p_list_file = fopen("data/p_list_task2.bin", "w");
	real_psi_p_list_file = fopen("data/real_psi_p_list_task2.bin", "w");
	imag_psi_p_list_file = fopen("data/imag_psi_p_list_task2.bin", "w");

	parameters_file = fopen("data/parameters_task2.bin", "w");

	fwrite(x_list, x_list_length * sizeof(double), 1, x_list_file);
	fwrite(real_psi_x_list, x_list_length * nbr_timesteps * sizeof(double), 1, real_psi_x_list_file);
	fwrite(imag_psi_x_list, x_list_length * nbr_timesteps * sizeof(double), 1, imag_psi_x_list_file);

	fwrite(p_list, x_list_length * sizeof(double), 1, p_list_file);
	fwrite(real_psi_p_list, x_list_length * nbr_timesteps * sizeof(double), 1, real_psi_p_list_file);
	fwrite(imag_psi_p_list, x_list_length * nbr_timesteps * sizeof(double), 1, imag_psi_p_list_file);

	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);

	fclose(x_list_file);
	fclose(real_psi_x_list_file);
	fclose(imag_psi_x_list_file);

	fclose(p_list_file);
	fclose(real_psi_p_list_file);
	fclose(imag_psi_p_list_file);

	fclose(parameters_file);
}

void task3(double alpha, double T0) {
	/* Declare and initiate some variables */
	int x_index, time_index;  // For looping over

	double timestep = 0.1; // [fs]
	int nbr_timesteps = 4000;

	double d = 0.5; // Wavepacket width [A]
	double V0 = 0.1; // [eV]
	double x; // Wavefunction's position coordinate [A]
	double x0 = -8.0; // Mean position [A]
	double x_max = 40.0; // x goes from x0 - x_max to x0 + x_max [A]

	int x_list_length = ipow(2, 14); // List length 2^n - 1 optimal for FFT
	double x_list[x_list_length]; // List of positions [A]
	double V_list[x_list_length]; // List of potential energies [eV]
	double complex psi_x_list[x_list_length]; // psi(x) [A^(-1/2)]
	double *real_psi_x_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Real part of psi(x), real_psi_x_list[x_index + x_list_length * time_index], [A^(-1/2)]
	double *imag_psi_x_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Imaginary part of psi(x), imag_psi_x_list[x_index + x_list_length * time_index], [A^(-1/2)]
	double dx = 2.0 * x_max / x_list_length;
	double normalization = 2.0 * x_max / (x_list_length * sqrt(2.0 * PI * hbar)); // For normalizing psi(p)

	double p0 = sqrt(2.0 * hydrogen_mass * T0); // Mean momentum obtained from solving p0^2 / 2m = 0.1 eV, [eV fs / A]
	double p_list[x_list_length]; // List of momenta [eV fs / A]
	double complex psi_p_list[x_list_length]; // psi(p) [(eV fs / A)^(-1/2)]
	double *real_psi_p_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Real part of psi(p), real_psi_p_list[x_index + x_list_length * time_index], [(eV fs / A)^(-1/2)]
	double *imag_psi_p_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Imaginary part of psi(p), imag_psi_x_list[x_index + x_list_length * time_index], [(eV fs / A)^(-1/2)]

	int nbr_parameters = 5;
	double parameters[] = {x0, x_max, x_list_length, timestep, nbr_timesteps};

	FILE *x_list_file;
	FILE *real_psi_x_list_file;
	FILE *imag_psi_x_list_file;
	
	FILE *p_list_file;
	FILE *real_psi_p_list_file;
	FILE *imag_psi_p_list_file;

	char filename[50];
	FILE *parameters_file;


	/* Calculate potential energy and wavefunction in position basis at time 0 */
	for(x_index = 0; x_index < x_list_length; x_index++) {
		x = x0 + x_max * 2.0 * (x_index - x_list_length / 2.0) / x_list_length;
		x_list[x_index] = x;

		psi_x_list[x_index] = psi_x(x, x0, p0, d);
		real_psi_x_list[x_index + x_list_length * 0] = creal(psi_x_list[x_index]);
		imag_psi_x_list[x_index + x_list_length * 0] = cimag(psi_x_list[x_index]);

		V_list[x_index] = V_task3(x, V0, alpha);
	}

	/* Calculate wavefunction in momentum basis at time 0 */
	fft(psi_x_list, psi_p_list, x_list_length);
	fft_freq(p_list, dx, x_list_length);
	for(x_index = 0; x_index < x_list_length; x_index++) {
		p_list[x_index] *= 2.0 * PI * hbar; // Scale p to have the right unit

		real_psi_p_list[x_index + x_list_length * 0] = creal(psi_p_list[x_index]) * normalization;
		imag_psi_p_list[x_index + x_list_length * 0] = cimag(psi_p_list[x_index]) * normalization;
	}


	/* Time evolve the state */
	for (time_index = 1; time_index < nbr_timesteps; time_index++) {

		/* Perform a single timestep */
		perform_timestep_task2(psi_x_list, x_list, x_list_length, dx, V_list, timestep);

		/* Calculate wavefunction in momentum basis by using FFT */
		fft(psi_x_list, psi_p_list, x_list_length);

		/* Save real and imaginary parts of wavefunction separately for writing to file */
		for(x_index = 0; x_index < x_list_length; x_index++) {
			real_psi_x_list[x_index + x_list_length * time_index] = creal(psi_x_list[x_index]);
			imag_psi_x_list[x_index + x_list_length * time_index] = cimag(psi_x_list[x_index]);
			real_psi_p_list[x_index + x_list_length * time_index] = creal(psi_p_list[x_index]) * normalization;
			imag_psi_p_list[x_index + x_list_length * time_index] = cimag(psi_p_list[x_index]) * normalization;
		}
	}


	/* Write lists to file */
	snprintf (filename, sizeof(filename), "data/x_list_alpha=%.1f_T0=%.2f_task3.bin", alpha, T0);
	x_list_file = fopen(filename, "w");
	snprintf (filename, sizeof(filename), "data/real_psi_x_list_alpha=%.1f_T0=%.2f_task3.bin", alpha, T0);
	real_psi_x_list_file = fopen(filename, "w");
	snprintf (filename, sizeof(filename), "data/imag_psi_x_list_alpha=%.1f_T0=%.2f_task3.bin", alpha, T0);
	imag_psi_x_list_file = fopen(filename, "w");

	snprintf (filename, sizeof(filename), "data/p_list_alpha=%.1f_T0=%.2f_task3.bin", alpha, T0);
	p_list_file = fopen(filename, "w");
	snprintf (filename, sizeof(filename), "data/real_psi_p_list_alpha=%.1f_T0=%.2f_task3.bin", alpha, T0);
	real_psi_p_list_file = fopen(filename, "w");
	snprintf (filename, sizeof(filename), "data/imag_psi_p_list_alpha=%.1f_T0=%.2f_task3.bin", alpha, T0);
	imag_psi_p_list_file = fopen(filename, "w");

	parameters_file = fopen("data/parameters_task3.bin", "w");

	fwrite(x_list, x_list_length * sizeof(double), 1, x_list_file);
	fwrite(real_psi_x_list, x_list_length * nbr_timesteps * sizeof(double), 1, real_psi_x_list_file);
	fwrite(imag_psi_x_list, x_list_length * nbr_timesteps * sizeof(double), 1, imag_psi_x_list_file);

	fwrite(p_list, x_list_length * sizeof(double), 1, p_list_file);
	fwrite(real_psi_p_list, x_list_length * nbr_timesteps * sizeof(double), 1, real_psi_p_list_file);
	fwrite(imag_psi_p_list, x_list_length * nbr_timesteps * sizeof(double), 1, imag_psi_p_list_file);

	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);

	fclose(x_list_file);
	fclose(real_psi_x_list_file);
	fclose(imag_psi_x_list_file);

	fclose(p_list_file);
	fclose(real_psi_p_list_file);
	fclose(imag_psi_p_list_file);

	fclose(parameters_file);

	/* Free some memory */
	free(real_psi_x_list);
	free(imag_psi_x_list);
	free(real_psi_p_list);
	free(imag_psi_p_list);
}

void task5(double T0) {
	/* Declare and initiate some variables */
	int x_index, time_index;  // For looping over

	double timestep = 2.0; // [fs]
	int nbr_timesteps = 60;

	double c = 0.1; // coupling strength [eV]
	double d = hbar / sqrt(4 * hydrogen_mass * 0.1 * T0); // Wavepacket width [A]
	// double V0 = 0.1; // [eV]
	double x; // Wavefunction's position coordinate [A]
	double x0 = -5.0; // Mean position [A]
	double x_max = 30.0; // x goes from x0 - x_max to x0 + x_max [A]

	int x_list_length = ipow(2, 12); // List length 2^n - 1 optimal for FFT
	double x_list[x_list_length]; // List of positions [A]
	double complex psi1_x_list[x_list_length]; // psi1(x) [A^(-1/2)]
	double complex psi2_x_list[x_list_length]; // psi2(x) [A^(-1/2)]
	double *real_psi1_x_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Real part of psi(x), real_psi_x_list[x_index + x_list_length * time_index], [A^(-1/2)]
	double *real_psi2_x_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Real part of psi(x), real_psi_x_list[x_index + x_list_length * time_index], [A^(-1/2)]
	double *imag_psi1_x_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Imaginary part of psi(x), imag_psi_x_list[x_index + x_list_length * time_index], [A^(-1/2)]
	double *imag_psi2_x_list = malloc(x_list_length * nbr_timesteps * sizeof(double)); // Imaginary part of psi(x), imag_psi_x_list[x_index + x_list_length * time_index], [A^(-1/2)]
	double dx = 2.0 * x_max / x_list_length;
	double normalization = 2.0 * x_max / (x_list_length * sqrt(2.0 * PI * hbar)); // For normalizing psi(p)

	double p0 = sqrt(2.0 * hydrogen_mass * T0); // Mean momentum obtained from solving p0^2 / 2m = 0.1 eV, [eV fs / A]
	double p_list[x_list_length]; // List of momenta [eV fs / A]
	double complex psi1_p_list[x_list_length]; // psi(p) [(eV fs / A)^(-1/2)]
	double complex psi2_p_list[x_list_length]; // psi(p) [(eV fs / A)^(-1/2)]

	int nbr_parameters = 7;
	double parameters[] = {x0, x_max, x_list_length, T0, c, timestep, nbr_timesteps};

	FILE *x_list_file;
	FILE *real_psi1_x_list_file;
	FILE *real_psi2_x_list_file;
	FILE *imag_psi1_x_list_file;
	FILE *imag_psi2_x_list_file;
	
	char filename[50];
	FILE *parameters_file;


	/* Initiate x_list */
	for(x_index = 0; x_index < x_list_length; x_index++) {
		x = x0 + x_max * 2.0 * (x_index - x_list_length / 2.0) / x_list_length;
		x_list[x_index] = x;
	}

	/* Initialize psi as T (psi1, 0) */
	initiate_psi_task5(psi1_x_list, psi2_x_list, x_list, x0, d, p0, x_list_length);
	for(x_index = 0; x_index < x_list_length; x_index++) {
		real_psi1_x_list[x_index + x_list_length * 0] = creal(psi1_x_list[x_index]);
		real_psi2_x_list[x_index + x_list_length * 0] = creal(psi2_x_list[x_index]);
		imag_psi1_x_list[x_index + x_list_length * 0] = cimag(psi1_x_list[x_index]);
		imag_psi2_x_list[x_index + x_list_length * 0] = cimag(psi2_x_list[x_index]);
	}

	/* Time evolve the state */
	for (time_index = 1; time_index < nbr_timesteps; time_index++) {

		/* Perform a single timestep */
		perform_timestep_task5(psi1_x_list, psi2_x_list, x_list, x_list_length, dx, c, timestep);
		// printf("%f + i%f\n", creal(psi1_x_list[0]), cimag(psi1_x_list[0])); // BODGE

		/* Save real and imaginary parts of wavefunction separately for writing to file */
		for(x_index = 0; x_index < x_list_length; x_index++) {
			real_psi1_x_list[x_index + x_list_length * time_index] = creal(psi1_x_list[x_index]);
			real_psi2_x_list[x_index + x_list_length * time_index] = creal(psi2_x_list[x_index]);
			imag_psi1_x_list[x_index + x_list_length * time_index] = cimag(psi1_x_list[x_index]);
			imag_psi2_x_list[x_index + x_list_length * time_index] = cimag(psi2_x_list[x_index]);
		}
	}


	/* Write lists to file */
	snprintf (filename, sizeof(filename), "data/x_list_c=%.2f_T0=%.2f_task5.bin", c, T0);
	x_list_file = fopen(filename, "w");
	snprintf (filename, sizeof(filename), "data/real_psi1_x_list_c=%.2f_T0=%.2f_task5.bin", c, T0);
	real_psi1_x_list_file = fopen(filename, "w");
	snprintf (filename, sizeof(filename), "data/real_psi2_x_list_c=%.2f_T0=%.2f_task5.bin", c, T0);
	real_psi2_x_list_file = fopen(filename, "w");
	snprintf (filename, sizeof(filename), "data/imag_psi1_x_list_c=%.2f_T0=%.2f_task5.bin", c, T0);
	imag_psi1_x_list_file = fopen(filename, "w");
	snprintf (filename, sizeof(filename), "data/imag_psi2_x_list_c=%.2f_T0=%.2f_task5.bin", c, T0);
	imag_psi2_x_list_file = fopen(filename, "w");

	parameters_file = fopen("data/parameters_task5.bin", "w");

	fwrite(x_list, x_list_length * sizeof(double), 1, x_list_file);
	fwrite(real_psi1_x_list, x_list_length * nbr_timesteps * sizeof(double), 1, real_psi1_x_list_file);
	fwrite(real_psi2_x_list, x_list_length * nbr_timesteps * sizeof(double), 1, real_psi2_x_list_file);
	fwrite(imag_psi1_x_list, x_list_length * nbr_timesteps * sizeof(double), 1, imag_psi1_x_list_file);
	fwrite(imag_psi2_x_list, x_list_length * nbr_timesteps * sizeof(double), 1, imag_psi2_x_list_file);

	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);

	fclose(x_list_file);
	fclose(real_psi1_x_list_file);
	fclose(real_psi2_x_list_file);
	fclose(imag_psi1_x_list_file);
	fclose(imag_psi2_x_list_file);

	fclose(parameters_file);

	/* Free some memory */
	free(real_psi1_x_list);
	free(real_psi2_x_list);
	free(imag_psi1_x_list);
	free(imag_psi2_x_list);
}

int main() {
	// int i; // For looping over

	// int nbr_cases = 6; // to be used in loop with task 3
	// double alphas[] = {0.5, 0.5, 0.5, 2.0, 2.0, 2.0}; // [A]
	// double T0s[] = {0.08, 0.10, 0.12, 0.08, 0.10, 0.12}; // [eV]

	// task1();

	// task2();

	// for (i = 0; i < nbr_cases; i++) {
	// 	task3(alphas[i], T0s[i]);
	// }
	
	double a = 0.3;
	task5(1 * a);

	return 0;
}