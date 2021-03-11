#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<getopt.h>
#include<omp.h>
#include<time.h>
#include<complex.h>
#include<threads.h>
//#include"c11threads.h"

int NUM_THREADS = 1;
int NUM_ROWS = 1;
int EXPONENT = 1;
// conv can max me 49 and attract can max be 9
u_int8_t *convergences;
u_int8_t *attractors;
u_int8_t *line_complete;
double *roots;
int next_line = 0;

typedef struct {
  mtx_t *mtx;
} thrd_info_t;


void parse_arg(int argc, char *argv[]);
void find_next_point(double *real, double *imag);
void find_roots(){
    if (0 < EXPONENT && EXPONENT < 10){
        for(int ix = 0; ix < EXPONENT; ix++){
            roots[2*ix]   = cos ((double)ix*6.28318531/(double)EXPONENT);
            roots[2*ix+1] = sin ((double)ix*6.28318531/(double)EXPONENT);
        }
    }else
        printf("Invalid exponent chosen\n");
        
}

int compute_thread(void *args){
  /*struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  long s = spec.tv_sec;
  long ms = round(spec.tv_nsec / 1.0e6);*/

  u_int8_t attractor[NUM_ROWS];
  u_int8_t convergence[NUM_ROWS];
  int current_line = 0;
  const thrd_info_t *thrd_info = (thrd_info_t*) args;

  //distribute lines to handle by this thread, loop will run once for every line
  while(1){
    mtx_lock(thrd_info->mtx);
    if(next_line < NUM_ROWS){
      current_line = next_line;
      next_line++;
      mtx_unlock(thrd_info->mtx);
    }else {
      mtx_unlock(thrd_info->mtx);
      break;
    }
    //Every thread can have its own copy of the roots for faster/noncompeting access?
    double roots_local[2*EXPONENT];
    for(int ix = 0; ix < EXPONENT; ix++){
        roots_local[2*ix] = roots[2*ix];
        roots_local[2*ix+1] = roots[2*ix+1];
    }
    double ycoordinate = 2 - 4*(double)current_line/(double)NUM_ROWS;
    //FOR EVERY POINT ON THE LINE DO
    for(int ix = 0; ix < NUM_ROWS; ix++){
        double real = -2 + 4*(double)ix/(double)NUM_ROWS;
        double imag = ycoordinate;
        //printf("coordinates for new point: %f %f\n", real, imag);
        u_int8_t attr = 10; //attractor 0-8 are the 9 zeroes. index 9 is the non-conv "zero"
        u_int8_t conv = 0;
        //ITERATION
        for(conv = 0; ; conv++){
            double x2 = real*real + imag*imag;
            //printf("coordinates current iter: %f %f %f\n", real, imag, x2);
            if(real > 10000000000.0 || real < -10000000000.0 ||
               imag > 10000000000.0 || imag < -10000000000.0 ||
               x2 < 0.000001){
                attr = 9;
                break;
            }
            //if norm is within 0.1% we check convergence
            //if(0.9979 < x2 && x2 < 1.0021){
                for(u_int8_t jx = 0; jx < EXPONENT; jx++){
                    double diff_real = roots_local[2*jx]-real;
                    double diff_imag = roots_local[2*jx+1]-imag;
                    double dist_from_root_2 = diff_real*diff_real + diff_imag*diff_imag;
                    if(dist_from_root_2 < 0.000001){
                        //printf("Here real %f imag %f dist from root %f\n", real, imag, dist_from_root_2);
                        attr = jx;
                        break;
                    }
                }
            //}   
            if(attr != 10){
                break;
            }
            //update real and imag
            find_next_point(&real, &imag);

        }
        //printf("writing to convergances and attractors at %i conv: %i attr: %i\n\n", current_line*NUM_ROWS+ix, conv, attr);
        if (conv > 49)
            conv = 49;
        convergence[ix] = conv;
        attractor[ix] = attr;
    }
    for(int ix = 0; ix < NUM_ROWS; ix++){
        convergences[current_line*NUM_ROWS + ix] = convergence[ix];
        attractors[current_line*NUM_ROWS + ix] = attractor[ix];
    }
    //memcpy(attractors + current_line*NUM_ROWS, attractor, sizeof(u_int8_t)*NUM_ROWS);
    //memcpy(convergence + current_line*NUM_ROWS, convergence, sizeof(u_int8_t)*NUM_ROWS);
    line_complete[current_line] = 1;
  }
    /*clock_gettime(CLOCK_MONOTONIC, &spec);
    long s2 = spec.tv_sec;
    long ms2 = round(spec.tv_nsec / 1.0e6);
    printf("Time of compute: %f\n",(float)(s2-s+(float)(ms2-ms)/1000.0));*/
  return 0;
}

int write_thread(void *args){
  FILE *fp_attractor;
	FILE *fp_convergence; 
  char filename[100];
  char attractor_s[NUM_ROWS*6+1];
  char convergence_s[NUM_ROWS*3+1];
  char colors[10][6] = 
    {
        "2 0 0 ","0 2 0 ","0 0 2 ","2 2 0 ","2 0 2 ","0 2 2 ","3 1 0 ","2 1 0 ", "0 1 3 ", "3 3 3 "
    };
  char greys[50][3] = 
    {
        "00 ","01 ","02 ","03 ","04 ","05 ","06 ","07 ", "08 ", "09 ",
        "10 ","11 ","12 ","13 ","14 ","15 ","16 ","17 ", "18 ", "19 ",
        "20 ","21 ","22 ","23 ","24 ","25 ","26 ","27 ", "28 ", "29 ",
        "30 ","31 ","32 ","33 ","34 ","35 ","36 ","37 ", "38 ", "39 ",
        "40 ","41 ","42 ","43 ","44 ","45 ","46 ","47 ", "48 ", "49 ",
    };
  sprintf(filename,"newton_attractors_x%d.ppm",EXPONENT);
  fp_attractor = fopen(filename, "w"); 
  sprintf(filename,"newton_convergence_x%d.ppm",EXPONENT);
	fp_convergence = fopen(filename, "w");
  //write header
  fprintf(fp_attractor, "P3\n%d %d\n3\n", NUM_ROWS, NUM_ROWS);
  fprintf(fp_convergence, "P2\n%d %d\n50\n", NUM_ROWS, NUM_ROWS);
  //write to files row by row
  /*struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  long s = spec.tv_sec;
  long ms = round(spec.tv_nsec / 1.0e6);*/
  for(int ix = 0; ix < NUM_ROWS; ix++){
    while(!line_complete[ix])
      nanosleep((struct timespec[]){{0, 100000}}, NULL);
    for(int jx = 0; jx < NUM_ROWS; jx++){
      // copy the 6 bytes that represent the color corresponding to that attractor root into attractor_s
      memcpy(attractor_s + 6*jx, colors[*(attractors + ix*NUM_ROWS + jx)],6*sizeof(char));
      memcpy(convergence_s + 3*jx, greys[*(convergences + ix*NUM_ROWS + jx)],3*sizeof(char));
    }

    attractor_s[NUM_ROWS*6+1] = '\n';
    convergence_s[NUM_ROWS*3+1] = '\n';
    fwrite(attractor_s, sizeof(char)*6*NUM_ROWS, 1, fp_attractor);
    fwrite(convergence_s, sizeof(char)*3*NUM_ROWS, 1, fp_convergence);

  }

  /*clock_gettime(CLOCK_MONOTONIC, &spec);
  long s2 = spec.tv_sec;
  long ms2 = round(spec.tv_nsec / 1.0e6);
  printf("Time of write: %f",(float)(s2-s+(float)(ms2-ms)/1000.0));*/

  fclose(fp_attractor);
  fclose(fp_convergence);
  return 0;
}

int main(int argc, char *argv[])
{
    parse_arg(argc, argv);

    line_complete = calloc(NUM_ROWS, sizeof(u_int8_t));
    attractors = calloc(NUM_ROWS*NUM_ROWS, sizeof(u_int8_t));
    convergences = calloc(NUM_ROWS*NUM_ROWS, sizeof(u_int8_t));
    roots = calloc(20, sizeof(double));

    find_roots();

    mtx_t mtx;
    mtx_init(&mtx, mtx_plain);

    thrd_t compute_t[NUM_THREADS];
    thrd_info_t compute_t_info[NUM_THREADS];
    thrd_t write_t;


    for(size_t ix = 0; ix < NUM_THREADS; ix++){
        compute_t_info[ix].mtx = &mtx;
        int r = thrd_create(compute_t+ix, compute_thread, (void*)(compute_t_info + ix));
        if ( r != thrd_success ) {
            fprintf(stderr, "failed to create thread\n");
            exit(1);
        }
    }
    int r = thrd_create(&write_t, write_thread,(void*)0);
    if ( r != thrd_success ) {
        fprintf(stderr, "failed to create thread\n");
        exit(1);
    }

    for(size_t ix = 0; ix < NUM_THREADS; ix++){
       int r;
       thrd_join(compute_t[ix], &r);
    }
    thrd_join(write_t, &r);

    free(line_complete);
    free(convergences);
    free(attractors);
    free(roots);
    void mtx_destroy(mtx_t *mtx);
}



void parse_arg(int argc, char *argv[]){
    int opt;
        while((opt = getopt(argc, argv, "t:l::")) != -1)  
        {  
            switch(opt)  
            {  
                case 't':
                    if(optarg == NULL)
                        printf("NO VALUE RECOGNIZED FOR %c FLAG\n", opt);
                    NUM_THREADS = atoi(optarg);
                    break;
                case 'l': 
                    if(optarg == NULL)
                        printf("NO VALUE RECOGNIZED FOR %c FLAG\n", opt);
                    NUM_ROWS = atoi(optarg);
                    break;  
                case '?':
                    printf("Unidentified flag %c\n", opt);
            }  
        } 
    EXPONENT = atoi(argv[optind]);
}

void find_next_point(double *real, double *imag){
    //double re0 = *real;
    //double im0 = *imag;
    //double re02; double im02;
    double complex z = *real + *imag*I;
    double complex z2 = z*z;
    double complex z4 = z2*z2;
    switch (EXPONENT) {
        case 1:
            z = 1.0;
            break;
        case 2:
            z = 0.5*z+ 0.5/z;
            break;
			  case 3:
			      z = (z*2.0/3.0) + (1.0/(z2*3.0));
            break;
        case 4:
            z = (3.0*z/4.0) + 1.0/(4.0*z2*z);
            break;
        case 5:
            z = (4.0*z/5.0) + 1.0/(5.0*z4);
            break;
        case 6:
            z = (5.0*z/6.0) + 1.0/(6.0*z4*z);
            break;
        case 7:
            z = (6.0*z/7.0) + 1.0/(7.0*z4*z2);
            break;
        case 8:
            z = (7.0*z/8.0) + 1.0/(8.0*z4*z2*z);
            break;
        case 9:
            z = 1.0/(9.0*z4*z4) + 8.0*z/9.0;
            break;
        default:
            fprintf(stderr, "unexpected degree\n");
            exit(1);
    }
    *real = creal(z);
    *imag = cimag(z);

}
