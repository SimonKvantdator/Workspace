
CC = gcc
CFLAGS = -fopenmp -Wall -O3
LIBS = -lm -lgsl -lgslcblas

HEADERS = utility_funcs.h
OBJECTS = utility_funcs.o main.o
PROGRAM = program

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o
	touch *.c

