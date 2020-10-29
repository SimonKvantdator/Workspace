#include <stdio.h> // for printf
#include <stdlib.h> // for srand()
#include <time.h> // for random seed
#include <gsl/gsl_rng.h>
#include "functions.h"

/* declare some variables */
int dim;

void main()
{
    double u;
    const  gsl_rng_type *T; /*  static  info  about  rngs */
    gsl_rng *q; /* rng  instance  */
    gsl_rng_env_setup (); /*  setup  the  rngs */
    T = gsl_rng_default; /*  specify  default  rng */
    q = gsl_rng_alloc(T); /*  allocate  default  rng */
    gsl_rng_set(q,time(NULL)); /*  Initialize  rng */
    u = gsl_rng_uniform(q); /*  generate  random  number
    (repeatable) */
    gsl_rng_free(q); /*  deallocate  rng */
    printf("%f", u);

    printf("plox enter the number of dimensions which you want your vectors to have: ");
    scanf("%d", &dim);

    /* generate vectors a & b */
    double a[dim];
    double b[dim];
    srand((unsigned)time(NULL));
    for(int i = 0; i < dim; i++)
    {
        a[i] = (double) rand() / RAND_MAX * 2.0 - 1.0;
        b[i] = (double) rand() / RAND_MAX * 2.0 - 1.0;
    }

    /* print vectors */
    printf("a = ");
    printVector(a, dim);
    printf("b = ");
    printVector(b, dim);
    printf("<a, b> = %.2f \n", scalarProduct(a, b, dim));
}
