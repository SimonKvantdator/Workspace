CC = gcc
CFLAGS = -fopenmp -O3
LIBS = -lm -lgsl -lgslcblas 

HEADERS = utility_funcs.h my_fft_func.h
OBJECTS = utility_funcs.o my_fft_func.o main.o
PROGRAM = program

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o
	touch *.c