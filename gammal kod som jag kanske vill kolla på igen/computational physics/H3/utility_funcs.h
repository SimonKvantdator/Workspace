#include <complex.h> // Standard Library of Complex Numbers

double ipow(double a, int n);
double sum(double *array, int array_length);
double mean(double *array, int array_length);
double variance(double *array, int array_length);
double scalar_product(double v1[3], double v2[3]);
double distance(double r1[3], double r2[3]);
double norm(double r[3]);
double complex psi_x(double x, double x0, double p0, double d);
void perform_timestep_task2(double complex *psi_x_list, double *x_list, int x_list_length, double dx, double *potential_list, double timestep);
void matmul(double A[][2], double B[][2], double C[][2], int ROWS, int COLS);
void matrix_inverse(double A[2][2], double A_inv[2][2]);
void exp_task5(double complex psi1, double complex psi2, double V[2][2]);
void initiate_psi_task5(double complex *psi1, double complex *psi2, double *x_list, double x0, double d, double p0, int x_list_length);
void perform_timestep_task5(double complex *psi1_x_list, double complex *psi2_x_list, double *x_list, int x_list_length, double  dx, double c, double timestep);
double V_task2(double x);
double V_task3(double x, double V0, double alpha);
void V_task5(double V[2][2], double x);
void T_task5(double T[2][2], double x);
double heaviside(double x);
double sign(double x);