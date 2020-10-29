#include "functions.h"

double scalarProduct(double vector1[], double vector2[], int vectorLength)
{
    double scalarProduct = 0;

    for(int i = 0; i < vectorLength; i++)
    {
        scalarProduct += vector1[i] * vector2[i];
    }

    return scalarProduct;
}