#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<getopt.h>
#include<omp.h>


const int chunk_size = 24000000;

static inline void strrmc(char *str, char c){
	int writer = 0, reader = 0;
	while (str[reader]){
		if(str[reader] != c) str[writer++] = str[reader];
		reader++;
	}
	str[writer] = 0;
}
static inline int toi(char *str, size_t len)
    {
        int value_ = 0;
        int sign;
        sign = ',' - str[0];
        //instead of
        if (str[0] == '-')
            sign = -1;
				value_ += (str[1] - '0') * 10000;
				value_ += (str[2] - '0') * 1000;
				value_ += (str[3] - '0') * 100;
				value_ += (str[4] - '0') * 10;
				value_ += (str[5] - '0');
				value_ *= sign;
				return value_;
        free(str);
    }

int main(int argc, char *argv[])
{
    FILE *fp = fopen("cell_e5", "r");
    if (fp == NULL){
        printf("Could not open file cells.txt");
        return 1;
    }

    char *buffer = malloc(chunk_size*sizeof(char));
    int rows = chunk_size/24;
    int *numbers = malloc(3*rows*sizeof(float));

    //float *cell_elements = (float*) malloc(buffer_rows*3*sizeof(float));
    //fread(buffer, 1, buffer_size, fp);

    fread(buffer, 1, chunk_size, fp);
    strrmc(buffer, '.');
    //float numbers[buffer_rows*3];

        //char *token = strtok(buffer, " \n");
        //char *token2 = strtok(NULL, " \n");
        //char *token3 = strtok(NULL , " \n");
        for(int ix = 0; ix < rows; ix++)
            numbers[ix] = toi(buffer + ix*7, 5);

    printf("One element is: %i %i %i \n", numbers[0], numbers[1], numbers[2]);
    fclose(fp);
    free(buffer);
    free(numbers);
}
