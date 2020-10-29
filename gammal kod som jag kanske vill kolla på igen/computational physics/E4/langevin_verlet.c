#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "langevin_verlet.h"

#define PI (3.1415926535897932)
#define k_B (1.3880649 * 1e-2) /* "[J/K]"  [(10^(-12)g * (10⁻⁶m)²/(10⁻³s)²)]*/
#define mass (30.134) /* [10^(-12)g]*/
#define T (297)


double acc_calc(double x, double v, double tau){
	double omega = (2 * PI) * 3.0 * 1e3 * 1e-3; /*[1/ms]*/
	double out = -omega*omega * x;

	return out;
}

void langevin_verlet(double* x, double* v, double* a, double dt, double tau, double G1, double G2){
	
	double eta = 1.0 / tau;
	double c0 = exp(-eta * dt);
	double v_th = sqrt(k_B * T / mass);
	double v_new_half;

	v_new_half = 0.5 * *a * dt + sqrt(c0) * *v + v_th * sqrt(1 - c0) * G1;
	*x= *x + v_new_half * dt;
	*a = acc_calc(*x, v_new_half, tau);
	*v = 0.5 * sqrt(c0) * *a * dt + sqrt(c0) * v_new_half + v_th * sqrt(1-c0) * G2;

}





























