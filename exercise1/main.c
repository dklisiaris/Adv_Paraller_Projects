#include "loop.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	system("clear");
	loop_func(1,10);

	pthread_execute_loop(loop_func,20,30);

	return 0;
}
