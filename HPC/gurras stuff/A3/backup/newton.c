#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<getopt.h>
#include<omp.h>
#include <assert.h>
#include <threads.h>

int NUM_THREADS;
int NUM_ROWS;
int EXPONENT;

void parse_arg(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    parse_arg(argc, argv);
    thrd_t thrds[nthrds];
}

void parse_arg(int argc, char *argv[]){
    int opt;
        while((opt = getopt(argc, argv, "t:l::")) != -1)  
        {  
            switch(opt)  
            {  
                case 't':
                    printf("flag %c recognized with value %d:\n", opt, atoi(optarg));
                    if(optarg == NULL)
                        printf("NO VALUE RECOGNIZED FOR %c FLAG\n", opt);
                    NUM_THREADS = atoi(optarg);
                    break;
                case 'l': 
                    printf("flag %c recognized with value %d:\n", opt, atoi(optarg));
                    if(optarg == NULL)
                        printf("NO VALUE RECOGNIZED FOR %c FLAG\n", opt);
                    NUM_ROWS = atoi(optarg);
                    break;  
                case '?':
                    printf("Unidentified flag %c\n", opt);
            }  
        } 
        EXPONENT = atoi(argv[optind]);
        printf("Exponent recognized %d\n", EXPONENT);
    }

