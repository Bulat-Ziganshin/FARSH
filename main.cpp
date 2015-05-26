#include <stdio.h>
#include <stdint.h>
#include "timer.h"
#include "farsh.c"

int main()
{
    const size_t DATASIZE = 28*1024;
    const uint64_t DATASET = uint64_t(10)<<30;
    printf("Hashing %d GiB", int(DATASET>>30));
    Timer t;  t.Start();

    for (int i=0; i<DATASET/DATASIZE; i++)
    {
        static char data[DATASIZE];  data[0]=(char)i;
        auto h = farsh (data, DATASIZE);
        if (h==42) break;
    }

    t.Stop();  auto speed = DATASET/(t.Elapsed()/1000);
    printf(": %.3lf milliseconds = %.3lf GB/s = %.3lf GiB/s\n", t.Elapsed(), speed/1e9, speed/(1<<30));
    return 0;
}
