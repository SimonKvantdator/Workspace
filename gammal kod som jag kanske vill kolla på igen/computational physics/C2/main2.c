#include <stdio.h> // for printf
#include <stdlib.h> // for srand()
#include <time.h> // for random seed

double *array;
int arraySize;

void main()
{
    arraySize = 10000; // number of data points I wish to generate
    double array[arraySize];

    FILE *f = fopen("array.txt", "w");
    srand((unsigned)time(NULL));
    for (int i=0; i < arraySize-1; i++)
    {
        array[i] = (double) rand() / RAND_MAX * 2.0 - 1.0;
        fprintf(f, "%f, ", array[i]);
    }
    fprintf(f, "%f", array[arraySize]); //append last element withour ","
    fclose(f);
}