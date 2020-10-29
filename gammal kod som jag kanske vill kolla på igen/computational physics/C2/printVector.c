#include <stdio.h>
#include "functions.h"

void printVector(double vector[], int vectorLength)
{
    printf("[");
    for (int i = 0; i < vectorLength; i++)
    {
        printf(" %.2f", vector[i]);
    }
    printf(" ]\n");
}