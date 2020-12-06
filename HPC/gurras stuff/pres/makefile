# Usage:
# make
# make clean
# remove all binaries and objects

.PHONY : all clean

CFLAGS = -lm -g -O0 -march=native

SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

all: ${BINS}

#%: %.o
#	@echo "Checking..."
#${CC} ${LINKERFLAG} $< -o $@

% : %.c
	@echo "Creating object..."
	gcc ${CFLAGS} -o $@ $< 

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}
	
