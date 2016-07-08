#include <stdio.h>
#include "CpuID.h"

int main()
{
    struct CpuidFeatures features;
    GetCpuidFeatures(&features);
    printf("%s: %s\n", features.IDString, features.HighestSuportedSimdString);
}
