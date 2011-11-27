#include "bench_functions.h"

/* Loop test functions*/

/*a very simple function to start with. Must be called with NULL arg*/
void loop_func(void *arg, int start, int stop)
{
	int * test_pt=(int *)arg;
	int i,test = *test_pt;
	for(i=start;i<=stop;i++)
	{
		test+=i;
		printf("i:%d test(test+i):%d\n",i,test);
		test-=i;
	}
	*test_pt=40;
}

/*a given test function which implements an array*/
void ploop_func(void *arg, int low, int high)
{
	double *A = (double *) arg;
	int loop_counter;

	for(loop_counter=low;loop_counter<=high;loop_counter++)
	{
		A[loop_counter]=loop_counter+10;
	}
}

/*function implements a vector addition*/
void vector_loop_func(void * arg, int low, int high)
{
	vectors * v = (vectors*) arg;
	
	int loop_counter;

	for(loop_counter=low;loop_counter<=high;loop_counter++)
	{
		v->C[loop_counter]=v->A[loop_counter]+v->B[loop_counter];
	}
}

double return_time(void)
{
	gettimeofday(&tm, NULL);
	double time=tm.tv_sec+(tm.tv_usec/1000000.0);
	
	return time;
}
