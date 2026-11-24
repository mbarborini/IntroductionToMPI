CC      := mpicc
CFLAGS  := -O3

SRCS    := $(wildcard *.c)
TARGETS := $(SRCS:.c=.x)

.PHONY: all clean

all: $(TARGETS)

%.x: %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -f $(TARGETS)