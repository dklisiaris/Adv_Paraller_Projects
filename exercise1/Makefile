#!/bin/bash

CC = gcc
files = bench_functions.c loop.c main.c
objects = bench_functions.o loop.o main.o
LIBS = -lm -pthread -g
OUTEXEC = outexec

CFLAGS = -W -Wall -Wextra

.PHONY: all
all: $(OUTEXEC)
	

$(OUTEXEC): $(objects)
	$(CC) $(CFLAGS) $(objects) $(LIBS) -o $@

main.o: main.c
	$(CC) -c -g main.c

bench_functions.o: bench_functions.c bench_functions.h
	$(CC) -c -g bench_functions.c

loop.o: loop.c loop.h
	$(CC) -c -g loop.c

.PHONY: clean
clean:
	@echo "all *.o *.out *~ files will be deleted !!!"
	rm -f *.o *.out *~

.PHONY: clean_all
clean_all:
	@echo "all *.o *.out *~ files will be deleted plus .txt files !!!!!"
	rm -f *.o *.out *~ *.txt

