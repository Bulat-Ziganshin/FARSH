#include <stdint.h>
#include "xxHash/xxhash.c"

void xxHash32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = XXH32(key,len,seed);
}
