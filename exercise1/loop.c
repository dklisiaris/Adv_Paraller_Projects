#include "loop.h"

int pthread_execute_loop(void (*func)(void *arg, int low, int high),void *arg, int policy, int chunk, int nthreads, int low, int high)
{
	if(nthreads<1 || (policy!=STATIC && policy!=DYNAMIC))return EINVAL;
	if(chunk==-1)chunk=1;

	pthread_t thread[nthreads];//definition of threads	
	int i,j;

	if(policy==STATIC)
	{
		pthread_data * pt_data;	

		/*allocate some memory for the thread structs*/
		pt_data=malloc(nthreads * (sizeof(pthread_data)));
		if(pt_data==NULL)
		{
			fprintf(stderr, "out of memory\n");
			exit(1);
		}

		set_up_pthread_data(func, arg, pt_data, nthreads, low, high);

		/*create the #nthreads threads with  and pt_data struct as argument*/
		for(i=0;i<nthreads;i++)
		{
			pthread_create(&thread[i],NULL, (void*) &stat_pfunc,(void*)&pt_data[i]);
		}
	}

	else if(policy==DYNAMIC)
	{	
		dynamic_pthread_data * dpt_data;
		dpt_data=malloc(nthreads * sizeof(dynamic_pthread_data));

		pthread_mutex_init(&mutex, NULL);

		set_up_dynamic_pthread_data(func, arg, dpt_data, chunk, nthreads, low, high);

		/*create the #nthreads threads with  and pt_data struct as argument*/
		for(i=0;i<nthreads;i++)
		{
			pthread_create(&thread[i],NULL, (void*) &dyn_pfunc,(void*)&dpt_data[i]);
		}
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

void set_up_dynamic_pthread_data
	(void(*func)(void *arg,int low,int high),void *arg, dynamic_pthread_data *dpd,int chunk, int nthreads, int init_low,int init_high)
{
	int i;
	for(i=0;i<nthreads;i++)
	{
		dpd[i].global_high=init_high;
		dpd[i].func=func;
		dpd[i].f_arg=arg;
		dpd[i].chunk=chunk;
		dpd[i].chunk_counter=&init_low;
	}
}

void stat_pfunc(void * arg)
{
	pthread_data * pd = (pthread_data *)arg;
	void (*func)(void*,int,int);
	func=pd->func;
	func(pd->f_arg, pd->low, pd->high);

}

void dyn_pfunc(void * arg)
{
	dynamic_pthread_data * dpd = (dynamic_pthread_data *)arg;
	int start,end;
	void (*func)(void*,int,int);
	func=dpd->func;
	while(*(dpd->chunk_counter)<dpd->global_high)
	{
		pthread_mutex_lock(&mutex);
		if(*(dpd->chunk_counter)>dpd->global_high)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		start=*(dpd->chunk_counter);
		end=start+dpd->chunk;
		*(dpd->chunk_counter)=end+1;
		if(*(dpd->chunk_counter)>dpd->global_high)end=dpd->global_high;
		pthread_mutex_unlock(&mutex);

		func(dpd->f_arg, start, end);
	}
}


