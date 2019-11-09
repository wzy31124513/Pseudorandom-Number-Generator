#include "stdlib.h"
#include "stdio.h"
#include <omp.h>
int a = 1103515245;
int m = 0x80000000;
int c = 12345;
int x0 = 1;
long k = 10;
double time;

int main()
{
	int count=1;
	for (k = 10; k < 100; k*=10)
	{

		long* lcg=malloc((k+1)*sizeof(long));
		lcg[0] = x0;
		time = omp_get_wtime();
		for (int i = 1; i < k+1; ++i)
		{
			lcg[i] = (a*lcg[i-1]+c)%m;
		}
		time = omp_get_wtime() - time;
		printf("k=10^%d,omp time %fs\n",count, time);
		count++;
		printf("[");
		for (int i = 0; i < k+1; ++i)
		{
			if (i!=k){
			printf("%ld,",lcg[i]);
		}else{
			printf("%ld",lcg[i]);
		}
		}
		printf("]\n");

		free(lcg);
	}


	return 0;
}