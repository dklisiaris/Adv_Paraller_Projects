#include "loop.h"
#include <stdio.h>
#include <stdlib.h>

int pthread_execute_loop(void (*func)(int low, int high),int low, int high)
{
/*	void (*fp)(int,int);

	fp=func;
	fp(2,5);
	(*fp)(3,9);*/
	func(low,high);
	return 0;
}

void loop_func(int start, int stop)
{
	int i;
	for(i=start;i<=stop;i++)
	{
		printf("i:%d\n",i);	
	}

}
