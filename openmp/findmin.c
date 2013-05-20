#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int len = 10;
	int a[] = {1, 2, 3, 4, -1, 6, 7, 8, 9, 10};
	int min = 1000;
	int tid;
	
	int sum = 0;
	int tmp;
	
	#pragma omp parallel for
	for (int i = 0; i < 10000; i++)
	{
		//tid = omp_get_thread_num();
		//printf("Thread %d process %d element\n", tid, i);
		//fflush(stdout);
		tmp = i;
		sum+=tmp;
		//if (a[i] < min)
			//min = a[i];
	}
	//printf("Min element is: %d\n", min);
	printf("Sum is: %d\n", sum);
	
	#pragma omp parallel
	{
		printf("Hello world OpenMP");
		printf("\n");
	}
}
