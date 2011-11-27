#include <stdio.h>
#include <stdlib.h>
#include "loop.h"
#include "bench_functions.h"

int main(int argc, char *argv[])
{
	int policy,chunk,num_of_threads;
	if(argc==4)
	{
		policy=atoi(argv[1]);
		chunk=atoi(argv[2]);
		num_of_threads=atoi(argv[3]);
	}
	else
	{
		policy=STATIC,chunk=20,num_of_threads=4;
	}
	
	system("clear");
	//loop_func(1,10);
	int t=10;
	int *p=&t;
	
	//pthread_execute_loop(loop_func,(void *) p, STATIC,-1,4,20,30);
	//printf("t: %d\n",t);

	double A[1500];
	//ploop_func((void*)A,100,200);

	double t1=return_time();
	pthread_execute_loop(ploop_func,(void *)A, policy,chunk,num_of_threads,10,1400);
	double t2=return_time();
	
	printf("Calculation Completed... [%.6lf] \n", t2-t1);
	printf("A[120]:%f\n",A[120]);

	return 0;
}
