CC=gcc
CFLAGS=-g -Wall -Wextra -std=c99

SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)
TARGETS=$(OBJS:%.o=%)

build: $(OBJS)
	$(CC) $(CFLAGS) -o vma $(OBJS)
	
run_vma: build
	./vma

clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: pack clean	
