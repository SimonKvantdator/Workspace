BINS = newton
CFLAGS = -O2 -march=native -lpthread -lm

.PHONY : all
all : $(BINS) 

newton: newton.c
	gcc $(CFLAGS) -o $@ $<
	#musl-gcc $(CFLAGS) -o $@ $< # FOR MY OWN MACHINE

.PHONY : clean
clean :
	rm -rf $(BINS)
