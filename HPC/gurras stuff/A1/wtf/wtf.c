#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(){
    const size_t loops = 10;
    const int size = pow(2,20);
    struct timespec ts1, ts2;
    FILE *fp;
    char filename[20] = "ints.txt";
    int *ints = (int *) malloc(sizeof(int) * size);
    int *ints2 = (int *) malloc(sizeof(int) * size);

    // generate entries
    for(size_t i = 0; i < size; ++i)
        ints[i] = i;

    // write
    timespec_get(&ts1, TIME_UTC);
    fp = fopen(filename, "w");
    for(size_t i = 0; i < loops; ++i)
    {   
        fwrite(ints, sizeof(int), size, fp);
    }
    fclose(fp);    
    timespec_get(&ts2, TIME_UTC);
    double diff = (ts2.tv_sec + ts2.tv_nsec * 1.0e-9) - (ts1.tv_sec + ts1.tv_nsec * 1.0e-9);
    printf("ms elapsed for write: %f\n",diff * 1.e3);
    fflush(fp); // ???????????????


    //read
    timespec_get(&ts1, TIME_UTC);
    fp = fopen(filename, "r");
    for(size_t i = 0; i < loops; ++i)
    {
        fread(ints2, sizeof(int), size, fp);
    }
    fclose(fp); 
    timespec_get(&ts2, TIME_UTC);
    diff = (ts2.tv_sec + ts2.tv_nsec * 1.0e-9) - (ts1.tv_sec + ts1.tv_nsec * 1.0e-9);
    printf("ms elapsed for read: %f\n",diff * 1.e3);
    fflush(fp); //???????????????????
}

