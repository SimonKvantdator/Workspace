
CC = gcc
CFLAGS = -O3 -Wall
LIBS = -lm -lgsl -lgslcblas 

HEADERS = initfcc.h alpotential.h utility_funcs.h verlet_funcs.h fft_func.h
OBJECTS = initfcc.o alpotential.o utility_funcs.o verlet_funcs.o fft_func.o MD_main.o
PROGRAM = program

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o
	touch *.c

