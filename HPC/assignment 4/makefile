COMPILER = gcc
BINS = heat_diffusion
INCLUDE = libOpenCL.so
FLAGS = -O2 -march=native -lm

.PHONY : all
all : $(BINS) 

heat_diffusion: heat_diffusion.c
	$(COMPILER) $(FLAGS) $(INCLUDE) -o $@ $<
#musl-gcc $(CFLAGS) -o $@ $< # FOR MY OWN MACHINE

.PHONY : clean
clean :
	rm -rf $(BINS)
