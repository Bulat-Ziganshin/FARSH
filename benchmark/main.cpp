#include <stdio.h>
#include <stdint.h>
#include "timer.h"
#if defined(FARSH_AVX2) || defined(FARSH_SSE2)
#include "CpuID.h"
#endif

#include "../farsh.c"

#if __GNUC__
#define ALIGN(n)      __attribute__ ((aligned(n)))
#elif _MSC_VER
#define ALIGN(n)      __declspec(align(n))
#else
#define ALIGN(n)
#endif

int main (int argc, char **argv)
{
    bool print_table = argc>1;      // if any cmdline parameter was given
    bool x64 = (sizeof(void*)==8);  // check for 64-bit platform

#ifdef FARSH_AVX2
    char simdext[] = "-avx2";
    struct CpuidFeatures features;  GetCpuidFeatures(&features);
    if (! features.AVX2)  {if (!print_table) printf("AVX2 not found!\n"); return 1;}
#elif defined(FARSH_SSE2)
    const char *simdext = x64? "":"-sse2";
    struct CpuidFeatures features;  GetCpuidFeatures(&features);
    if (! features.SSE2)  {if (!print_table) printf("SSE2 not found!\n"); return 1;}
#else
    const char *simdext = x64? "-nosimd":"";
#endif

#ifdef FARSH_ALIGNED_INPUT
    bool ALIGNED_INPUT = true;
#else
    bool ALIGNED_INPUT = false;
#endif

    char progname[100];
    sprintf (progname, "%sfarsh-%s%s", ALIGNED_INPUT? "aligned-":"",
                                       x64? "x64":"x86",
                                       simdext);

    // CHECK THE ZEROES HASHING
    const size_t ZEROES = 64*1024;
    ALIGN(64) static char zero[ZEROES] = {0};
    for (int i=0; i<=ZEROES; i++)
    {
        //uint32_t h = farsh (zero, i);
        //printf("%5d %08x\n", i, h);
        //printf("%4d %08x %08x %08x %08x :: ", minbytes, (UINT)(h), (UINT)(h>>32), sum1, sum2);
    }


    // PREPARE TEST DATA. DATASIZE+FARSH_BASE_KEY_SIZE should be less than the L1 cache size, otherwise speed may be limited by memory reads
    const size_t DATASIZE = 12*1024;
    ALIGN(64) static char data_array[DATASIZE+1];
    char *data  =  ALIGNED_INPUT? data_array : data_array + 1;
    for (int i=0; i<DATASIZE; i++)
        data[i] = char((123456791u*i) >> ((i%16)+8));


#ifndef FARSH_ALIGNED_INPUT
    // CHECK FOR POSSIBLE DATA ALIGNMENT PROBLEMS
    for (int i=0; i<=64; i++)
    {
        uint32_t h = farsh (data+i, DATASIZE+1-i, 0);
        if (h==42)  break;   // anti-optimization trick

        char out[32*4];
        for (int j=1; j<=32; j++)
            farsh_n (data+i, DATASIZE+1-i, 0, j, 0, out);
    }
#endif


    // BENCHMARK
    const uint64_t DATASET = uint64_t(100)<<30;
    if (print_table)  printf("%-24s  | ", progname);
    else              printf("Hashing %d GiB:", int(DATASET>>30));
    const int EXTRA_LOOPS = (100<<20) / DATASIZE;   // These extra loops are required to enable the SIMD engine and switch CPU core to the maximum frequency
    Timer t;


    for (int i=0; i < EXTRA_LOOPS+DATASET/DATASIZE; i++)
    {
        if (i == EXTRA_LOOPS)
            t.Start();

        uint32_t h = farsh (data, DATASIZE, 0);

        if (h != 0xd300ddd8) {   // check hash correctness
            printf("\nWrong hash value at iteration %d: %08x !!!\n", i, h);
            return 2;
        }
    }
    t.Stop();  double speed = DATASET / t.Elapsed();
    if (print_table)  printf(                     " %6.3lf GB/s = %6.3lf GiB/s  ",                   speed/1e9, speed/(1<<30));
    else              printf(" %.3lf milliseconds = %6.3lf GB/s = %6.3lf GiB/s\n", t.Elapsed()*1000, speed/1e9, speed/(1<<30));


    const uint32_t *keys = FARSH_KEYS;
    if (t.Elapsed() == 1e42)   data++, keys++;   // anti-optimization trick

    if (print_table)  printf("| ");
    else              printf("Internal loop:");
    t.Start();
    for (int i=0; i < DATASET/FARSH_BASE_KEY_SIZE; i++)
    {
        uint64_t h = farsh_full_block ((uint32_t*)data, keys);
        if (h==42)  data[0] = i;    // anti-optimization trick
    }
    t.Stop();  speed = DATASET / t.Elapsed();
    if (print_table)  printf(                       " %6.3lf GB/s = %6.3lf GiB/s\n",                   speed/1e9, speed/(1<<30));
    else              printf("   %.3lf milliseconds = %6.3lf GB/s = %6.3lf GiB/s\n", t.Elapsed()*1000, speed/1e9, speed/(1<<30));


    return 0;
}
