#ifndef LOOP_H
#define LOOP_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define STATIC 1
#define DYNAMIC 2

/*struct used by threads in static policy*/
typedef struct
{
	int low;
	int high;
	void (*func)(void *arg, int low, int high);
	void * f_arg;

}pthread_data;

/*struct used by threads in dynamic policy*/
typedef struct
{
	int global_high;
	void (*func)(void *arg, int low, int high);
	void * f_arg;
	int chunk;
	int * chunk_counter;

}dynamic_pthread_data;

/*defines the pthread mutex*/
pthread_mutex_t mutex;

int pthread_execute_loop(void (*func)(void *arg, int low, int high),void *arg, int policy, int chunk, int nthreads, int low, int high);

void set_up_pthread_data(void(*func)(void *arg,int low,int high),void *arg, pthread_data *pd, int nthreads, int init_low, int init_high);

void set_up_dynamic_pthread_data
	(void(*func)(void *arg,int low,int high),void *arg, dynamic_pthread_data *dpd,int chunk, int nthreads, int init_low,int init_high);

void stat_pfunc(void * arg);

void dyn_pfunc(void * arg);


#endif
