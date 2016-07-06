#include <stdio.h>
#include "CpuID.h"

void printregs(int eax,int ebx,int ecx,int edx){
  int j;
  char string[17];

  string[16] = '\0';
  for(j=0;j<4;j++){
    string[j] = eax >> (8*j);
    string[j+4] = ebx >> (8*j);
    string[j+8] = ecx >> (8*j);
    string[j+12] = edx >> (8*j);
  }
  printf("%s",string);
}

#define cpuid(in,a,b,c,d)\
  asm("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

int main()
{
    struct CpuidString str;
    struct CpuidFeatures features;
    GetCpuidString(&str);
    GetCpuidFeatures(&features);
//    printf("%12.12s\n", str.IDString);

//    printf("Extended brand string: \"");
    for (unsigned i=0x80000002;i<=0x80000004;i++){
      unsigned long eax,ebx,ecx,edx;

      cpuid(i,eax,ebx,ecx,edx); 
      printregs(eax,ebx,ecx,edx);
    }
    printf("\n");
}
