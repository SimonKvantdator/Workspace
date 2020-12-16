BINS = newton
CFLAGS = -O2 -march=native -lpthread -lm

.PHONY : all
all : $(BINS) 

newton: newton.c
	gcc $(CFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf $(BINS)
