#include <math.h>
#include "functions.h"

#define PI 3.14159265358979323846

int f = 2;
double a = 1.0;
double phi = 0.0;

double h(double t)
{
    return a * cos(2 * PI * f * t + phi);
}