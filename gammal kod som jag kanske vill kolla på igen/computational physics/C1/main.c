#include <stdio.h>
#include <math.h>

/* declare some variables */
#define ROWS 3
#define COLS 3
double x;
double y;

/* write down some prototypes */
double function(double, double);
void increment(double *, double);
void matmul(double (*)[COLS], double (*)[COLS], double (*)[COLS]);
void print_array();

int main()
{
    x = 3.2;
    y = 4.9;

    /* test function */
    printf("x^3 + sqrt(y) = %f \n \n", function(x, y));    

    /* test increment */
    double *xp = &x;
    printf("x = %.2f before increment \n", x);
    increment(xp, 1.0);
    printf("x = %.2f after increment \n \n", x);

    /* test matmul */
    double X[ROWS][COLS] = 
    {
        {1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0}
    };
    double Y[ROWS][COLS] = 
    {
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0}
    };
    double C[ROWS][COLS] = {0};
    matmul(X, Y, C);
    printf("X * Y = \n");
    print_array(C);
    
}

double function(double x, double y)
{
    return pow(x, 3) + sqrt(y);
}

void increment(double *x, double val)
{
    *x += val;
}

void matmul(double A[ROWS][COLS], double B[ROWS][COLS], double C[ROWS][COLS])
{
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

void print_array(double A[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf(" %.1f", A[i][j]);
        }
        printf("\n");
    }
}
