CC = gcc
BINS = locality
CFLAGS = -O0 
# -march=native -g

.PHONY : all
all : $(BINS)


locality : locality.c
	$(CC) -o $@ $< $(CFLAGS) 

.PHONY : clean
clean :
	rm -rf $(BINS)
