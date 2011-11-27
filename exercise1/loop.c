#include "loop.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>


int pthread_execute_loop(void (*func)(void *arg, int low, int high),void *arg, int nthreads, int low, int high)
{
	if(nthreads<1)return EINVAL;

	pthread_data * pt_data;	

	/*allocate some memory for the thread structs*/
	pt_data=malloc(nthreads * (sizeof(pthread_data)));
	if(pt_data==NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	set_up_pthread_data(func, arg, pt_data, nthreads, low, high);
	pthread_t thread[nthreads];//definition of threads

	int i,j;
	/*create the #nthreads threads with  and pt_info struct as argument*/
	for(i=0;i<nthreads;i++)
	{
		pthread_create(&thread[i],NULL, (void*) &stat_pfunc,(void*)&pt_data[i]);
	}
	
	/*join threads when they are finished*/
	for(j=0;j<nthreads;j++)
	{
		pthread_join(thread[j], NULL);
	}

	//func(low,high);
	return 0;
}

void set_up_pthread_data(void(*func)(void *arg,int low,int high),void *arg, pthread_data *pd, int nthreads, int init_low, int init_high)
{
	//Number of total iterations
	int N = init_high-init_low+1;
	//Number of iterations per thread
	int range = N/nthreads;
	//Extra iterations to be executed in case N%nthreads!=0 
	int extra_iters = N%nthreads;
	int i, temp_low=init_low,extra_flag=0;

	for(i=0;i<nthreads;i++)
	{
		if(extra_iters!=0)extra_flag=1;
		pd[i].low=temp_low;
		pd[i].high=temp_low+range-1+extra_flag;
		
		temp_low+=(range+extra_flag);
		if(extra_iters!=0)extra_iters--;
		extra_flag=0;

		pd[i].func=func;
		pd[i].f_arg=arg;
	}
}

void stat_pfunc(void * arg)
{
	pthread_data * pd = (pthread_data *)arg;
	void (*func)(void*,int,int);
	func=pd->func;
	func(pd->f_arg, pd->low, pd->high);

}

