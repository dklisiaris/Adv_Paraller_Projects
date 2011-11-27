#ifndef LOOP_H
#define LOOP_H

typedef struct
{
	int low;
	int high;
	void (*func)(void *arg, int low, int high);
	void * f_arg;

}pthread_data;

int pthread_execute_loop(void (*func)(void *arg, int low, int high),void *arg, int nthreads, int low, int high);

void set_up_pthread_data(void(*func)(void *arg,int low,int high),void *arg, pthread_data *pd, int nthreads, int init_low, int init_high);

void stat_pfunc(void * arg);

#endif
