#include <stdio.h>
#include <stdlib.h>
#include "loop.h"
#include "bench_functions.h"

int main(void)
{
	system("clear");
	//loop_func(1,10);
	int t=10;
	int *p=&t;
	
	//pthread_execute_loop(loop_func,(void *) p, STATIC,-1,4,20,30);
	//printf("t: %d\n",t);

	double A[500];
	//ploop_func((void*)A,100,200);

	pthread_execute_loop(ploop_func,(void *)A, DYNAMIC,20,2,10,200);
	printf("A[120]:%f\n",A[120]);

	return 0;
}
