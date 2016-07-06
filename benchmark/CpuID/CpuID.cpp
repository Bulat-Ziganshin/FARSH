#include <stdio.h>
#include "CpuID.h"

int main()
{
    struct CpuidFeatures features;
    GetCpuidFeatures(&features);
    printf("%s: %s\n", features.IDString,
                       features.AVX? "AVX" : 
                       features.AESInput? "AES-NI" : 
                       features.SSE42? "SSE 4.2" : 
                       features.SSE41? "SSE 4.1" : 
                       features.SupplSSE3? "Supplemental SSE3" : 
                       features.SSE3? "SSE3" : 
                       features.SSE2? "SSE2" : 
                       features.SSE?   "SSE" : "no SSE");
}
