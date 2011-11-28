#include "loop.h"

/* function which executes loops in paraller*/
int pthread_execute_loop(void (*func)(void *arg, int low, int high),void *arg, int policy, int chunk, int nthreads, int low, int high)
{
	/*if an argument is wrong function returns einval*/
	if(nthreads<1 || (policy!=STATIC && policy!=DYNAMIC))return EINVAL;
	if(chunk==-1)chunk=1;

	pthread_t thread[nthreads];//definition of threads	
	int i,j;
	/* definitions of structs*/
	pthread_data * pt_data;
	dynamic_pthread_data * dpt_data;

	if(policy==STATIC)
	{			
		/*allocate some memory for the thread structs*/
		pt_data=malloc(nthreads * (sizeof(pthread_data)));
		if(pt_data==NULL)
		{
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
		/*init thread structs with proper data*/
		set_up_pthread_data(func, arg, pt_data, nthreads, low, high);

		/*create the #nthreads threads with  and pt_data struct as argument*/
		for(i=0;i<nthreads;i++)
		{
			pthread_create(&thread[i],NULL, (void*) &stat_pfunc,(void*)&pt_data[i]);
		}
	}

	else if(policy==DYNAMIC)
	{	/*allocate some memory for the thread structs*/
		dpt_data=malloc(nthreads * sizeof(dynamic_pthread_data));

		pthread_mutex_init(&mutex, NULL); //mutex initialization

		/*init thread structs with proper data*/
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
	/*deallocate some memory*/
	if(policy==STATIC)free(pt_data);
	else if(policy==DYNAMIC)free(dpt_data);
	return 0;
}

/*initialize threads data structs with proper data*/
void set_up_pthread_data(void(*func)(void *arg,int low,int high),void *arg, pthread_data *pd, int nthreads, int init_low, int init_high)
{
	//Number of total iterations
	int N = init_high-init_low+1;
	//Number of iterations per thread
	int range = N/nthreads;
	//Extra iterations to be executed in case N%nthreads!=0 
	int extra_iters = N%nthreads;
	int i, temp_low=init_low,extra_flag=0;

	/*this algorithms finds low and high for each thread. If N%P!=0 distributes work equally between threads*/
	for(i=0;i<nthreads;i++)
	{
		if(extra_iters!=0)extra_flag=1;
		pd[i].low=temp_low;
		pd[i].high=temp_low+range-1+extra_flag;
		
		temp_low+=(range+extra_flag);
		if(extra_iters!=0)extra_iters--;
		extra_flag=0;

		pd[i].func=func; //function pointer
		pd[i].f_arg=arg; //function argument pointer
	}
}

/*initialize threads data structs with proper data*/
void set_up_dynamic_pthread_data
	(void(*func)(void *arg,int low,int high),void *arg, dynamic_pthread_data *dpd,int chunk, int nthreads, int init_low,int init_high)
{
	int i;
	/*give to each thread the necessary data*/
	for(i=0;i<nthreads;i++)
	{
		dpd[i].global_high=init_high;
		dpd[i].func=func;
		dpd[i].f_arg=arg;
		dpd[i].chunk=chunk;
		dpd[i].chunk_counter=&init_low;
	}
}

/*func called by threads in static policy*/
void stat_pfunc(void * arg)
{
	pthread_data * pd = (pthread_data *)arg;
	void (*func)(void*,int,int);
	/*run the func pointed by thread data*/
	func=pd->func;
	func(pd->f_arg, pd->low, pd->high);

}
/*func called by threads in dynamic policy*/
void dyn_pfunc(void * arg)
{
	dynamic_pthread_data * dpd = (dynamic_pthread_data *)arg;
	int start,end;
	void (*func)(void*,int,int);
	func=dpd->func;
	/*runs until counter reaches the high end*/
	while(*(dpd->chunk_counter)<dpd->global_high)
	{	
		/*lock the mutex so only one thread can access counter*/
		pthread_mutex_lock(&mutex);
		/*if counter has reached end unluck the mutex and break*/
		if(*(dpd->chunk_counter)>dpd->global_high)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		/*start equals the current counter value*/
		start=*(dpd->chunk_counter);
		/*end equals the start plus chunk*/
		end=start+dpd->chunk;
		/*new chunk value is the last itteration of this thread plus one*/
		*(dpd->chunk_counter)=end+1;
		/*in case counter reached end then the end of this thread will be the global end*/
		if(*(dpd->chunk_counter)>dpd->global_high)end=dpd->global_high;
		/*unlock mutex so other threads can access counter*/
		pthread_mutex_unlock(&mutex);

		/*run the func with the dynamically specificed start and end*/
		func(dpd->f_arg, start, end);
	}
}


