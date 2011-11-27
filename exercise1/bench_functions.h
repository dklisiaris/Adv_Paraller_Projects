#ifndef BENCH_FUNCTIONS_H
#define BENCH_FUNCTIONS_H

void loop_func(void *arg, int start, int stop);

void ploop_func(void *arg, int low, int high);

typedef struct
{
	int *A;
	int *B;
	int *C;
}vectors;

void vector_loop_func(void * arg, int low, int high);

#endif
