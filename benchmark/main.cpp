#include <stdio.h>
#include <stdint.h>
#include "timer.h"
#include "../farsh.c"

#if __GNUC__
#include <x86intrin.h>
#define ALIGN(n)      __attribute__ ((aligned(n)))
#elif _MSC_VER
#include <intrin.h>
#define ALIGN(n)      __declspec(align(n))
#else
#define ALIGN(n)
#endif

int main()
{
    // CHECK THE ZEROES HASHING
    const size_t ZEROES = 64*1024;
    ALIGN(64) static char zero[ZEROES] = {0};
    for (int i=0; i<=ZEROES; i++)
    {
        //unsigned h = farsh (zero, i);
        //printf("%5d %08x\n", i, h);
        //printf("%4d %08x %08x %08x %08x :: ", minbytes, (UINT)(h), (UINT)(h>>32), sum1, sum2);
    }


    // PREPARE TEST DATA. DATASIZE+STRIPE should be less than the L1 cache size, otherwise speed may be limited by memory reads
    const size_t DATASIZE = 12*1024;
    ALIGN(64) static char data_array[DATASIZE+1];
#ifdef ALIGNED_DATA
        char *data = data_array;
#else
        char *data = data_array + 1;
#endif
    for (int i=0; i<DATASIZE; i++)
        data[i] = char((123456791u*i) >> ((i%16)+8));


#ifndef ALIGNED_DATA
    // CHECK FOR POSSIBLE DATA ALIGNMENT PROBLEMS
    for (int i=0; i<=64; i++)
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
    const int EXTRA_LOOPS = (100<<20) / DATASIZE;   // These extra loops are required to enable the SIMD engine and switch CPU core to the maximum frequency
    Timer t;

    for (int i=0; i < EXTRA_LOOPS+DATASET/DATASIZE; i++)
    {
        unsigned h = farsh (data, DATASIZE, 0);

        if (h != 0xd300ddd8) {   // check hash correctness
            printf("\nWrong hash value at iteration %d: %08x !!!\n", i, h);
            return 1;
        }

        if (i == EXTRA_LOOPS)
            t.Start();
    }

    t.Stop();  double speed = DATASET / t.Elapsed();
    printf(": %.3lf milliseconds = %.3lf GB/s = %.3lf GiB/s\n", t.Elapsed()*1000, speed/1e9, speed/(1<<30));
    return 0;
}
