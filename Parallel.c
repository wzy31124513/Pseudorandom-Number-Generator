#include "stdlib.h"
#include "stdio.h"
#include <omp.h>
int a = 1103515245;
int m = 0x80000000;
int c = 12345;
int x0 = 1;
long k = 10;
double time;
long mult_n=1103515245;
int theradnum;
static int dirty[128];
static long pseed[128];
long random_last;
long iseed;

#pragma omp threadprivate(random_last)
long random(){
    long random_next;
    int t = omp_get_thread_num();
    if (dirty[t]==1)
    {
        dirty[t]=0;
    }else{
        random_next = (long)(mult_n * random_last+c)%m;
        random_last=random_next;
    }
    return random_last;
}


int main()
{
    int count=1;

    for ( k = 10; k < 10000000000; k*=10)
    {
        long* seq=malloc((k+1)*sizeof(long));
        seq[0] = x0;
        double time = omp_get_wtime();
        #pragma omp parallel
        {
        #pragma omp single
        {
        	int theradnum=omp_get_num_threads();
            for (int i = 0; i < theradnum; ++i)
            {
                dirty[i]=1;
            }
            iseed = x0;
            seq[0]=x0;
            pseed[0]=iseed;
            for (int i = 1; i < theradnum; ++i)
            {
                iseed = (long)(a*iseed+c)%m;
                mult_n=(a*mult_n+c)%m;
                pseed[i]=iseed;
            }

        }
        random_last = pseed[omp_get_thread_num()];

        #pragma omp for schedule(static)
        for (int i = 0; i < k+1; ++i)
        {
            seq[i]=random();
        }
        }
        time = omp_get_wtime() - time;
        /*printf("[");
        for (int i = 0; i < k+1; ++i)
        {
            if (i!=k){
                printf("%ld,",seq[i]);
            }else{
                printf("%ld",seq[i]);
            }
        }
        printf("]\n");*/

        printf("k=10^%d,omp time %fs\n",count, time);
        count++;
    }

	return 0;
}