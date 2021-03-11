BINS = incrementing_an_array
CFLAGS = -O2 -lpthread

.PHONY : all
all : $(BINS) 

incrementing_an_array : incrementing_an_array.c
	gcc $(CFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf $(BINS)

