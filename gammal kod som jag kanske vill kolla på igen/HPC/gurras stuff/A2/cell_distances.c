#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<getopt.h>
#include<omp.h>

// CHANGES can be made on line ; FILEOPEN SOURCE
// CHANGES can be made on line ; BYTE SIZE BUFFER
static inline void strrmc(char str[], char c);
static inline void print_dist(int density[]);
static inline int parse_arg(int argc, char *argv[]);

int NUM_THREADS = 1;
const int chunk_size = 2400000;

inline int toi(char *str, size_t len)
    {
        int value_ = 0;
        int sign = 1;
        if (str[0] == '-') { // handle negative
            sign = -1;
        }
				value_ += (str[1] - '0') * 10000;
				value_ += (str[2] - '0') * 1000;
				value_ += (str[3] - '0') * 100;
				value_ += (str[4] - '0') * 10;
				value_ += (str[5] - '0');
				value_ *= sign;
				return value_;
    }


int main(int argc, char *argv[])
{  
    if(parse_arg(argc,argv) == 0){
        //printf("No argument passed for threads. Use -t <> for x threads.\n");
    }else
    sscanf(argv[1], "-t%i", &NUM_THREADS);
    omp_set_num_threads(NUM_THREADS);

    FILE *fp = fopen("cell_e4", "r");
    if (fp == NULL){
        printf("Could not open file cells.txt");
        return 1;
    }
    // SHOULD ONLY READ in SETS OF 24 to prevent line breaking?!
    //find out how long many bytes and therefore cells and lines
    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    int tot_rows = file_size/24; //24 bytes per row
    rewind(fp);

    // ONLY WANT EVEN ROWS
    int buffer_rows = chunk_size/24; 
    int buffer_size = buffer_rows*24;
    //printf("Chunk Size %d Bytes\n", chunk_size);
    //printf("Buffer rows %d\n", buffer_rows);
    //printf("Buffer size %d\n", buffer_size);
    //printf("File size %d\n", file_size);
    char *buffer = malloc(buffer_size);
    int *cell_main_elements = (int*) malloc(buffer_rows*3*sizeof(int));
    int **cells_main = (int**) malloc(buffer_rows*sizeof(int*));
    for(int ix = 0; ix < buffer_rows; ix++){
        cells_main[ix] = cell_main_elements + 3*ix;
    }
    //int **cells_main[buffer_rows][3] = malloc(buffer_rows*3, sizeof(int));
    int end_main = 0;
    int main_current_size = 0;
    int cells_temp[buffer_rows][3];
    int end_temp = 0;
    int temp_current_size = 0;
    // maximum lenth between points is 34.6410162 
    // with 4 digit accuracy that is 3465 points
    // saving as pointer for easy access
    int density[3465];
    for(int ix = 0; ix < 3465; ix++)
        density[ix] = 0;

    while(end_main < file_size){
        //set file pointer to end of main
        fseek(fp, end_main, SEEK_SET);
        // load in new main buffer
        main_current_size = fread(buffer, 1, buffer_size, fp);
	    end_main += main_current_size;       
        // Adding nr of successful reads to end_main
        strrmc(buffer, '.');
        // removing . and reading as int
        char *token = strtok(buffer, " \n");

        for(int ix = 0; ix < main_current_size/24; ix++){
            cells_main[ix][0] = toi(token, 5);
            token = strtok(NULL, " \n");
            cells_main[ix][1] = toi(token, 5);
            token = strtok(NULL, " \n");
            cells_main[ix][2] = toi(token, 5);
            token = strtok(NULL, " \n");
        }
        //check all points in main
        
        //printf("main current size %d\n", main_current_size);
        #pragma omp parallel for schedule(static, 32) reduction(+:density[:3465])
        for(int ix = 0; ix < main_current_size/24; ix++){
            for(int jx = (ix + 1); jx < main_current_size/24; jx++){
                int x1 = cells_main[ix][0]; 
                int y1 = cells_main[ix][1];
                int z1 = cells_main[ix][2];
                int x2 = cells_main[jx][0]; 
                int y2 = cells_main[jx][1];
                int z2 = cells_main[jx][2];
                int dx = x1 - x2;
                int dy = y1 - y2;
                int dz = z1 - z2;
                int dist = (int) (1e-1f * sqrtf(dx*dx + dy*dy + dz*dz));
                //int dist = (int) rintf(sqrtf(dx*dx + dy*dy + dz*dz) / 10);
                density[dist]++; 
            }
        }

        end_temp = end_main;
        //reading in temp
        while(end_temp < file_size){
            // load in new tmp buffer
            temp_current_size = fread(buffer, 1, buffer_size, fp);
            end_temp += temp_current_size;            
            //printf("End temp  %d\n", end_temp);
            // Adding nr of successful reads to end_temp
            strrmc(buffer, '.');
            // removing . and reading as int
            char *token = strtok(buffer, " \n");
            for(int ix = 0; ix < temp_current_size/24; ix++){
                cells_temp[ix][0] = atoi(token);
                token = strtok(NULL, " \n");
                cells_temp[ix][1] = atoi(token);
                token = strtok(NULL, " \n");
                cells_temp[ix][2] = atoi(token);
                token = strtok(NULL, " \n");
            }

            //check from every point in main to every in temp
            #pragma omp parallel for schedule(static, 32) reduction (+:density[:3465])
            for(int ix = 0; ix < main_current_size/24; ix++){
                for(int jx = 0 ; jx < temp_current_size/24; jx++){
                int x1 = cells_main[ix][0]; 
                int y1 = cells_main[ix][1];
                int z1 = cells_main[ix][2];
                int x2 = cells_temp[jx][0]; 
                int y2 = cells_temp[jx][1];
                int z2 = cells_temp[jx][2];
                int dx = x1 - x2;
                int dy = y1 - y2;
                int dz = z1 - z2;
                int dist = (int) (1e-1f * sqrtf(dx*dx + dy*dy + dz*dz));
                density[dist]++; 
                }
            }
        }
    }

    //printf("array element 0 and 1578 %d %d \n",density[0], density[1578]);
    print_dist(density);
    fclose(fp);
    return 0;  

}
static inline void strrmc(char str[], char c){
	int writer = 0, reader = 0;
	while (str[reader]){
		if(str[reader] != c) str[writer++] = str[reader];
		reader++;
	}
	str[writer] = 0;
}
static inline void print_dist(int density[]){
    FILE *fp2 = fopen("distances.txt", "w");
    double l = 0;
    for(int ix = 0; ix < 3465; ix++){
        if(density[ix] > 0){
            l = (double)ix/100; 
            //fprintf(fp2, "%.2f %d\n", l, density[ix]);
            printf("%.2f %d\n", l, density[ix]);


        }
    }
    fclose(fp2);
}
int parse_arg(int argc, char *argv[]){
    int opt;
        while((opt = getopt(argc, argv, "t:")) != -1)  
        {  
            switch(opt)  
            {  
                case 't':
                  //printf("flag %c recognized with value %d:\n", opt, atoi(optarg));
                  return atoi(optarg);
                  break;
                /*case ‘:’:  
                    printf(“option needs a value\n”);  
                    break;  
                case ‘?’:  
                    printf(“unknown option: %c\n”, optopt); 
                    break;
                */
                  
            }  
        }  
        return 0;
    }

