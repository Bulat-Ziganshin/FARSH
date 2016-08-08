#include <stdint.h>
#include "SpookyHash/SpookyV2.cpp"

void SpookyHash32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = SpookyHash::Hash32(key,len,seed);
}

void SpookyHash32a_test ( const void * key, int len, unsigned seed, void * out )
{
  uint64_t seed_out[2];
  seed_out[0] = seed;
  seed_out[1] = 0;
  SpookyHash::Hash128(key,len,seed_out,seed_out+1);
  *(uint32_t*)out  =  seed_out[0] >> 32;
}

void SpookyHash32b_test ( const void * key, int len, unsigned seed, void * out )
{
  uint64_t seed_out[2];
  seed_out[0] = seed;
  seed_out[1] = 0;
  SpookyHash::Hash128(key,len,seed_out,seed_out+1);
  *(uint32_t*)out  =  seed_out[1];
}

void SpookyHash32c_test ( const void * key, int len, unsigned seed, void * out )
{
  uint64_t seed_out[2];
  seed_out[0] = seed;
  seed_out[1] = 0;
  SpookyHash::Hash128(key,len,seed_out,seed_out+1);
  *(uint32_t*)out  =  seed_out[1] >> 32;
}

void SpookyHash64_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint64_t*)out = SpookyHash::Hash64(key,len,seed);
}

void SpookyHash128_test ( const void * key, int len, unsigned seed, void * out )
{
  uint64_t *seed_out = (uint64_t*) out;
  seed_out[0] = seed;
  seed_out[1] = 0;
  SpookyHash::Hash128(key,len,seed_out,seed_out+1);
}
