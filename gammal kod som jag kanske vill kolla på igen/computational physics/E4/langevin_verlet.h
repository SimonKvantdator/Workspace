

#ifndef _langevin_verlet_h
#define _langevin_verlet_h

	double acc_calc(double x, double v, double tau);
	void langevin_verlet(double* x, double* v, double* a, double dt, double tau, double G1, double G2);

#endif
