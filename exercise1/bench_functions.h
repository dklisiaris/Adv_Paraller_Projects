#ifndef BENCH_FUNCTIONS_H
#define BENCH_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>			/* header file for time functions*/
#include <sys/time.h>		/* header file for time functions*/
#include <math.h>

void loop_func(void *arg, int start, int stop);

void ploop_func(void *arg, int low, int high);

typedef struct
{
	int *A;
	int *B;
	int *C;
}vectors;

void vector_loop_func(void * arg, int low, int high);

void init_vectors(void * arg, int size, int range);

/*struct holding secs and microsecs. used by the time functions*/
struct timeval tm;

double return_time(void);

void one_milli(void);

void comp(int millis);

#endif
