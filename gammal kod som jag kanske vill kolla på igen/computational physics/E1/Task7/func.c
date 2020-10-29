/*
E1_func.c
 
Created by AL on 2013-10-24.
Further developed by Martin Gren on 2015-10-23.
*/


/*
Function that calculates the acceleration based on the Hamiltonian.
The acceleration is calculated based on the displacements q and then stored in a.
m, q, and a should be vectors of the same size, size_of_q
*/
void calc_acc(double *a, double *q, double *m, double kappa, int size_of_q)
{
    /* Declaration of variables */
    int i;
    
    /* Calculating the acceleration on the boundaries */
    a[0] = kappa*(-q[0] + q[1])/m[0];
    a[size_of_q - 1] = kappa*(q[size_of_q - 2] - q[size_of_q - 1])/m[size_of_q - 1];
    
    /* Calculating the acceleration of the inner points */
    for (i = 1; i < size_of_q - 1; i++){
        a[i] = kappa*(q[i - 1] - 2*q[i] + q[i + 1])/m[i];
    }
}

/* Function that calculates the potential energy based on the displacements */
double calc_pe(double *q, double kappa, int size_of_q)
{
    /* Declaration of variables */
    int i;
    double e = 0;
    /* Calculating the energy on the boundaries */
    e += kappa*((q[0] - q[1])*(q[0] - q[1])/2);
    
    /* Calculating the energy of the inner points */
    for (i = 1; i < size_of_q - 1; i++){
        e += kappa*(q[i] - q[i + 1])*(q[i] - q[i + 1])/2;
    }
    return e;	
}


/* Function that calculates and returns the kinetic energy based on the velocities and masses */
double calc_ke(double *v, int size_of_v, double *m)
{
    /* Declaration of variables */
    int i;
    double e = 0; 
    /* Calculating the energy of the inner points */
    for (i = 0; i < size_of_v; i++){
        e += m[i]*(v[i])*(v[i])/2;
    }
    return e;	
}
