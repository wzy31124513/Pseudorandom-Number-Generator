#include "stdlib.h"
#include "stdio.h"
#include <omp.h>
int a = 1103515245;
int m = 0x80000000;
int c = 12345;
int x0 = 1;
long k = 10;
double time;
long mult_n = 1103515245;
static long pseed[128];
long iseed;
int threadnum=8;

int main()
{
    int count=1;
    for ( k = 10; k < 10000000000; k*=10)
    {
        long* seq=malloc((k+1)*sizeof(long));
        seq[0] = x0;
        pseed[0]=x0;
        iseed = x0;
        mult_n=a;
        double time = omp_get_wtime();
        for (int i = 1; i < threadnum; ++i)
        {
            iseed = (long)(a*iseed+c)%m;
            mult_n=(a*mult_n+c)%m;
            pseed[i]=iseed;
        }

        long random_last[threadnum];
        for (int i = 0; i < threadnum; ++i)
        {
            random_last[i]=pseed[i];
        }
        int tn[threadnum];
        for (int i = 0; i < threadnum; ++i)
        {
            tn[i]=(k+1)/threadnum;
        }

        if ((k+1)%threadnum!=0)
        {
            for (int i = 0; i < threadnum; ++i)
            {
                tn[i]++;
                if ((k-i)%threadnum==0)
                {
                    break;
                }
            }
        }

        int index[threadnum];
        int idx=0;
        for (int i = 0; i < threadnum; ++i)
        {
            seq[idx]=pseed[i];
            index[i]=idx;
            idx+=tn[i];
        }

        for (int i = 0; i < threadnum; ++i)
        {
            for (int j = 1; j <tn[i]; j++)
            {
                if (index[i]+j<k)
                {
                    seq[index[i]+j] = (random_last[i]*mult_n+c)%m;
                    random_last[i]=seq[index[i]+j];
                }
            }
        }
       
        time = omp_get_wtime() - time;
        printf("k=10^%d,seq time %fs\n",count, time);
        count++;


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

    free(seq);
    }




	return 0;
}