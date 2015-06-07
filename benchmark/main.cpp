#include <stdio.h>
#include <stdint.h>
#include "timer.h"
#include "../farsh.c"

int main()
{
    // CHECK THE ZEROES HASHING
    const size_t ZEROES = 64*1024;
    static char zero[ZEROES] = {0};
    for (int i=0; i<=ZEROES; i++)
    {
        //unsigned h = farsh (zero, i);
        //printf("%5d %08x\n", i, h);
        //printf("%4d %08x %08x %08x %08x :: ", minbytes, (UINT)(h), (UINT)(h>>32), sum1, sum2);
    }


    // PREPARE TEST DATA. DATASIZE+STRIPE should be less than the L1 cache size, otherwise speed may be limited by memory reads
    const size_t DATASIZE = 28*1024;
    static char data[DATASIZE+1];
    for (int i=0; i<DATASIZE+1; i++)
        data[i] = char((123456791u*i) >> ((i%16)+8));


#ifndef ALIGNED_DATA
    // CHECK FOR POSSIBLE DATA ALIGNMENT PROBLEMS
    for (int i=0; i<=32; i++)
    {
        unsigned h = farsh (data+i, DATASIZE+1-i, 0);
        if (h==42)  break;   // anti-optimization trick

        char out[32*4];
        for (int j=1; j<=32; j++)
            farsh_n (data+i, DATASIZE+1-i, 0, j, 0, out);
    }
#endif


    // BENCHMARK
    const uint64_t DATASET = uint64_t(100)<<30;
    printf("Hashing %d GiB", int(DATASET>>30));
    Timer t;  t.Start();

    for (int i=0; i<DATASET/DATASIZE; i++)
    {
        data[1]=(char)i;
#ifdef ALIGNED_DATA
        unsigned h = farsh (data, DATASIZE, 0);
#else
        unsigned h = farsh (data+1, DATASIZE, 0);
#endif
        if (h==42)  break;
        if (i==42)  printf(" (%x)", h);
        //printf("\n %5d %08x ", i, h);
    }

    t.Stop();  double speed = DATASET/(t.Elapsed()/1000);
    printf(": %.3lf milliseconds = %.3lf GB/s = %.3lf GiB/s\n", t.Elapsed(), speed/1e9, speed/(1<<30));
    return 0;
}
