# Usage:
# make
# make clean
# remove all binaries and objects

.PHONY : clean

#LINKERFLAG = -lm

SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

#all: ${BINS}

#%: %.o
#	@echo "Checking..."
#${CC} ${LINKERFLAG} $< -o $@

same_file : same_file.c
	@echo "Creaing same_file..."
	gcc -o $@ $^ -g -O2

% : %.c
	@echo "Creating object..."
	gcc -o $@ $^ -g -O2

different_file : different_file.c different_file_mul.c
	@echo "Creating different file..."
	gcc -o $@ $^ -g -O2

inlined_manually : inlined_manually.c
	@echo "Creating inlined_manually..."
	gcc -o $@ $^ -g -O2

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}
	
