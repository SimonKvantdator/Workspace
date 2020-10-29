#include <math.h>
#include "functions.h"

#define PI 3.14159265358979323846

double h(double t, double f, double a, double phi)
{
    return a * cos(2 * PI * f * t + phi);
}