CC = gcc
CFLAGS = -fopenmp -O3 -Wall
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