#include <complex.h> // Standard Library of Complex Numbers
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "my_fft_func.h"
#include "utility_funcs.h"

#define PI (3.1415926535897932)
#define hbar (0.658212) // [eV fs]
#define hydrogen_mass (104.5) // [eV fs^2 / A^2]

/* Returns a^n */
double ipow(double a, int n) {
	int i;
	double tmp = 1.0;

	for (i = 0; i < n; i++) {
		tmp *= a;
	}

	return tmp;
}

/* Returns sum of array using Kahan summation algorithm. */
double sum(double *array, int array_length) {
	int i; // For summing over
	double term;
	double tmp_sum;
	double sum = 0.0; // Prepare the accumulator.
	double compensator = 0.0; // A running compensation for lost low-order bits.

	for (i = 0; i < array_length; i++) {
		term = array[i] - compensator;
		tmp_sum = sum + term;
		compensator = (tmp_sum - sum) - term;
		sum = tmp_sum;
	}

	return sum;
}

/* Returns mean of array. */
double mean(double *array, int array_length) {
	return sum(array, array_length) / (double) array_length;
}

/* Returns variance of array */
double variance(double *array, int array_length) {
	int i; // for summing over
	double tmp;
	double *array_squared = malloc(array_length * sizeof(double));

	for (i = 0; i < array_length; i++) {
		array_squared[i] = ipow(array[i], 2);
	}

	tmp = mean(array_squared, array_length) - ipow(mean(array, array_length), 2);
	free(array_squared);

	return tmp;
}

/* Returns scalar product of two vectors in R^3 */
double scalar_product(double v1[3], double v2[3]) {
	int i;
	double sum = 0.0;

	for(i = 0; i < 3; i++){
		sum += v1[i] * v2[i];
	}

	return sum;
}

/* Returns distance between two vectors in R^3 */
double distance(double r1[3], double r2[3]){
	int i;
	double norm;
	double r12[3];

	for(i = 0; i < 3; i++) {
		r12[i] = r1[i] - r2[i];
	}

	norm = sqrt(scalar_product(r12, r12));
	return norm;
}

/* Returns norm of vector in R^3 */
double norm(double r[3]) {
	return sqrt(scalar_product(r, r));
}

/********************** Problem specific functions **********************/

/* Gaussian wave packet in position basis */
double complex psi_x(double x, double x0, double p0, double d) {
	return pow(PI * ipow(d, 2), -1 / 4.0) * 
		exp(-ipow(x - x0, 2) / (2 * ipow(d, 2))) * 
		cexp(I * p0 * (x - x0) / hbar);
}

/*
	Performs one timestep of split operator FFT.
	Takes wavefunction psi_x, defined at positions x_list, at time t and returns wavefunction at time t + timestep in psi_x.
*/
void perform_timestep_task2(double complex *psi_x_list, double *x_list, int x_list_length, double dx, double *potential_list, double timestep) {
	int i; // For looping over
	double p_list[x_list_length];
	double p; // [eV fs / Å]

	double complex tmp_array1[x_list_length]; // temporary array for holding list to be Fourier transformed
	double complex tmp_array2[x_list_length]; // temporary array for holding list to be inverse Fourier transformed

	/* Initiate tmp_array1 */
	for (i = 0; i < x_list_length; i++) {
		tmp_array1[i] = cexp(-I * potential_list[i] * timestep / hbar) * psi_x_list[i];
	}

	/* Perform Fourier transform of tmp_array1 and store reslut in tmp_array2 */
	fft(tmp_array1, tmp_array2, x_list_length);
	fft_freq(p_list, dx, x_list_length);

	/* Update tmp_array2 with relevant factors */
	for (i = 0; i < x_list_length; i++) {
		p_list[i] *= 2 * PI * hbar; // Scale p to have the right unit
		
		p = p_list[i];
		tmp_array2[i] *= cexp(-I * ipow(p, 2) * timestep / (2 * hydrogen_mass * hbar));
	}

	/* Perform inverse Fourier transform of tmp_array2 and store reslut in psi_x */
	fft_inv(tmp_array2, psi_x_list, x_list_length);
}

/* Naive matrix multiplication */
void matmul(double **A, double **B, double **C, int ROWS, int COLS) {
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            for(int k = 0; k < COLS; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/* Naive 2x2 matrix inverse */
void matrix_inverse(double A[2][2], double A_inv[2][2])
{
	A_inv[0][0] = A[1][1] / (A[0][0] * A[1][1] - A[0][1] * A[1][0]);
	A_inv[0][1] = -A[0][1] / (A[0][0] * A[1][1] - A[0][1] * A[1][0]);
	A_inv[1][0] = -A[1][0] / (A[0][0] * A[1][1] - A[0][1] * A[1][0]);
	A_inv[1][1] = A[0][0] / (A[0][0] * A[1][1] - A[0][1] * A[1][0]);
}

/* Maps (psi1, psi2) to exp(V) (psi1, psi2) for a 2x2 matrix A */
void exp_task5(double complex psi1, double complex psi2, double V[2][2]) {
	int i; // For summing over

	double u1;
	double u2;
	double theta;
	double T[2][2];
	double T_inv[2][2];

	double V11 = V[0][0];
	double V12 = V[0][1];
	double V21 = V[1][0];
	double V22 = V[1][1];


	/* Find eigenvectors and eigenvalues of V */
	u1 = (1.0 / 2.0) * (V11 + V22 - sqrt(ipow(V11, 2) + 4.0 * V12 * V21 - 2.0 * V11 * V22 + ipow(V22, 2)));
	u2 = (1.0 / 2.0) * (V11 + V22 + sqrt(ipow(V11, 2) + 4.0 * V12 * V21 - 2.0 * V11 * V22 + ipow(V22, 2)));

	/* Basis transformation to where V is diagonal */
	if (V12 == 0.0) {
		theta = 0.0;
	} else {
		theta = atan((V11 + sqrt(4.0 * ipow(V12, 2) + ipow(V11 - V22, 2)) - V22) / (-2.0 * V12));
	}
	T[0][0] = cos(theta);
	T[0][1] = -sin(theta);
	T[1][0] = sin(theta);
	T[1][1] = cos(theta);
	matrix_inverse(T, T_inv);

	/*### Now, we use that exp(V) = T exp(D) T^-1 ###*/

	/* Act with T_inv on (psi1, psi2) */
	psi1 = T_inv[0][0] * psi1 + T_inv[0][1] * psi2;
	psi2 = T_inv[1][0] * psi1 + T_inv[1][1] * psi2;

	/* Act with exp(D) on (psi1, psi2) */
	psi1 = exp(u1) * psi1;
	psi2 = exp(u2) * psi2;

	/* Act with T on (psi1, psi2) */
	psi1 = T[0][0] * psi1 + T[0][1] * psi2;
	psi2 = T[1][0] * psi1 + T[1][1] * psi2;
	if (cabs(psi1) > 0.001) {
		printf("psi1[i] = %f\n", cabs(psi1));
	}

}

/* Initiate psi = (psi1, psi2) in diabatic representation */
void initiate_psi_task5(double complex *psi1, double complex *psi2, double *x_list, double x0, double d, double p0, int x_list_length) {
	int x_index; // For looping over
	double x; // [Å]
	double V[2][2]; // Potential matrix [eV]
	double V11;
	double V12;
	double V21;
	double V22;

	double theta; // angle between diabatic and adiabatic basis vectors
	double lambda1; // 1st eigenvalue of V [eV]
	double lambda2; // 2rd eigenvalue of V [eV]
	double T[2][2]; // transformation matrix from diabatic to adiabatic representation [1]
	double T_inv[2][2]; // TODO: this needed?


	/* Define psi as ground state in adiabatic representation */
	for (x_index = 0; x_index < x_list_length; x_index++) {
		x = x_list[x_index];

		psi1[x_index] = psi_x(x, x0, p0, d);
		psi2[x_index] = 0.0;
		

		/*### Switch basis to adiabatic representation ###*/
		/* Define potential V */
		V_task5(V, x);
		V11 = V[0][0];
		V12 = V[0][1];
		V21 = V[1][0];
		V22 = V[1][1];

		/* Diagonalize V */
		lambda1 = (1.0 / 2.0) * (V11 + V22 - sqrt(ipow(V11, 2) + 4.0 * V12 * V21 - 2.0 * V11 * V22 + ipow(V22, 2)));
		lambda2 = (1.0 / 2.0) * (V11 + V22 + sqrt(ipow(V11, 2) + 4.0 * V12 * V21 - 2.0 * V11 * V22 + ipow(V22, 2)));
		if (V12 == 0.0) {
			theta = 0.0;
		} else {
			theta = atan((V11 + sqrt(4.0 * ipow(V12, 2) + ipow(V11 - V22, 2)) - V22) / (2.0 * V12));
		}
		T[0][0] = cos(theta);
		T[0][1] = -sin(theta);
		T[1][0] = sin(theta);
		T[1][1] = cos(theta);
		// T[0][0] = -2 (4 V12^2+(V11-V22) (V11+Sqrt[4 V12^2+(V11-V22)^2]-V22)) (V11+Sqrt[4 V12^2+(V11-V22)^2];
		// T[0][1] = 4 V12 (4 V12^2+(V11-V22) (V11+Sqrt[4 V12^2+(V11-V22)^2]-V22));
		// T[1][0] = -4 V12 (4 V12^2+(V11-V22) (V11+Sqrt[4 V12^2+(V11-V22)^2]-V22));
		// T[1][1] = -2 (4 V12^2+(V11-V22) (V11+Sqrt[4 V12^2+(V11-V22)^2]-V22)) (V11+Sqrt[4 V12^2+(V11-V22)^2]-V22);
		// matrix_inverse(T, T_inv);


		/* Use this basis transformation to switch to adiabatic representation */
		// TODO: T or T_inv??
		psi1[x_index] = T[0][0] * psi1[x_index] + T[0][1] * psi2[x_index];
		psi2[x_index] = T[1][0] * psi1[x_index] + T[1][1] * psi2[x_index];
	}
}

void perform_timestep_task5(double complex *psi1_x_list, double complex *psi2_x_list, double *x_list, int x_list_length, double  dx, double c, double timestep) {
	int x_index; // For looping over
	double x; // [Å]
	double p_list[x_list_length];
	double p; // [eV fs / Å]

	double V[2][2]; // potential energy operator [eV]

	double complex psi1_tmp_arrayA[x_list_length]; // temporary array for holding list to be Fourier transformed
	double complex psi2_tmp_arrayA[x_list_length]; // temporary array for holding list to be Fourier transformed
	double complex psi1_tmp_arrayB[x_list_length]; // temporary array for holding list to be inverse Fourier transformed
	double complex psi2_tmp_arrayB[x_list_length]; // temporary array for holding list to be inverse Fourier transformed


	/* Initiate tmp_arrayA */
	for (x_index = 0; x_index < x_list_length; x_index++) {
	
		/* Get potential */
		x = x_list[x_index];
		V_task5(V, x);

		/* Act with exp(V) on (psi1(x), psi2(x)) */
		exp_task5(psi1_x_list[x_index], psi2_x_list[x_index], V);
		/* Act with exp(-i dt / hbar) on (psi1, psi2) */
		psi1_tmp_arrayA[x_index] = cexp(-I * timestep / hbar) * psi1_x_list[x_index];
		psi2_tmp_arrayA[x_index] = cexp(-I * timestep / hbar) * psi2_x_list[x_index];
	}

	/* Perform Fourier transform of tmp_array1 and store reslut in tmp_arrayB */
	fft(psi1_tmp_arrayA, psi1_tmp_arrayB, x_list_length);
	fft(psi2_tmp_arrayA, psi2_tmp_arrayB, x_list_length);
	fft_freq(p_list, dx, x_list_length);

	/* Update tmp_arrayB with relevant factors */
	for (x_index = 0; x_index < x_list_length; x_index++) {

		p_list[x_index] *= 2 * PI * hbar; // Scale p to have the right unit
		p = p_list[x_index];

		/* Act with exp(T) on tmp_arrayA */
		psi1_tmp_arrayB[x_index] *= cexp(-I * ipow(p, 2) * timestep / (2 * hydrogen_mass * hbar));
		psi2_tmp_arrayB[x_index] *= cexp(-I * ipow(p, 2) * timestep / (2 * hydrogen_mass * hbar));
	}

	/* Perform inverse Fourier transform of tmp_array2 and store reslut in psi_x */
	fft_inv(psi1_tmp_arrayB, psi1_x_list, x_list_length);
	fft_inv(psi2_tmp_arrayB, psi2_x_list, x_list_length);
}

/* Potential given for task 2 */
double V_task2(double x) {
	return 0.0;
}

/* Potential given for task 3 */
double V_task3(double x, double V0, double alpha) {
	return V0 / ipow(cosh(x / alpha), 2);
}

/* Returns potential given for task 5 in V */
void V_task5(double V[2][2], double x) {
	double a = 0.3; // [eV]
	double b = 0.4; // [Å]
	double c = 0.1; // [eV]
	double d = 0.7; // [Å]

	V[0][0] = a * (2.0 * heaviside(x) - sign(x) * exp(-sign(x) * x / b)); // [eV]
	V[0][1] = c * exp(-ipow(x / d, 2));
	V[1][0] = V[0][1];
	V[1][1] = 2 * a - V[0][0];
}

double heaviside(double x) {
	if (x > 0.0) {
		return 1.0;
	} else {
		return 0.0;
	}
}

double sign(double x) {
	if (x > 0.0) {
		return 1;
	} else {
		return -1.0;
	}
}