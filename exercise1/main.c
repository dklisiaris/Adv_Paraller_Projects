#include <stdio.h>
#include <stdlib.h>
#include "loop.h"
#include "bench_functions.h"

int main(int argc, char *argv[])
{
	int policy,chunk,num_of_threads;
	/*arguments given by user*/
	if(argc==4)
	{
		policy=atoi(argv[1]);
		chunk=atoi(argv[2]);
		num_of_threads=atoi(argv[3]);
	}
	else
	{
		policy=STATIC,chunk=20,num_of_threads=4; //default arguments
	}
	
	system("clear");

	double A[1500];
	//ploop_func((void*)A,100,200); <--sequantial func NOT USED

	/*use pthread execute function to execute ploop func in paraller and take measurements*/
	double t1=return_time();
	pthread_execute_loop(ploop_func,(void *)A, policy,chunk,num_of_threads,10,1400);
	double t2=return_time();
	
	printf("Calculation Completed... [%.6lf] \n", t2-t1);
	printf("A[120]:%f\n",A[120]);

	/*declare and initialize vectors*/
	vectors * vec=malloc(sizeof(vectors));
	init_vectors(vec, 1000, 50);

	//vector_loop_func(vec, 10, 900); <--sequantial func NOT USED

	/*use pthread execute function to execute vector ploop func in paraller and take measurements*/
	t1=return_time();
	pthread_execute_loop(vector_loop_func,(void *)vec, policy,chunk,num_of_threads,10,900);
	t2=return_time();
	
	printf("Calculation Completed... [%.6lf] \n", t2-t1);
	printf("A:%d+B:%d=C:%d\n",vec->A[200],vec->B[200],vec->C[200]);

	return 0;
}
