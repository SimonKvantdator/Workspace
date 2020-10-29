CC = gcc # c-compiler
CFLAGS = # compiler  flags  such as -O3 -W
LIBS = -lm -lgsl -lgslcblas # libraries  such as -lm -lgsl -lgslcblas

OBJECTS = main2.o scalarProduct.o printVector.o # object  files  to  produce
PROGRAM = program # name of  executable  to be  built
# build .o files
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
# compile  program
all: $(PROGRAM)
$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
clean:
	rm -f *.o
	touch *.c
