#include "Hashes.h"

#include "Random.h"


//----------------------------------------------------------------------------
// fake / bad hashes

void DoNothingHash ( const void *, int, uint32_t, void * )
{
}

//-----------------------------------------------------------------------------
// One-byte-at-a-time hash based on Murmur's mix

uint32_t MurmurOAAT ( const void * key, int len, uint32_t seed )
{
  const uint8_t * data = (const uint8_t*)key;

  uint32_t h = seed;

  for(int i = 0; i < len; i++)
  {
    h ^= data[i];
    h *= 0x5bd1e995;
    h ^= h >> 15;
  }

  return h;
}


//-----------------------------------------------------------------------------
// 32-bit parts of MurmurHash3_x86_128

void Murmur3c_32 ( const void * key, const int len, uint32_t seed, void * out )
{
  uint32_t full_out[4];
  MurmurHash3_x86_128 ( key, len, seed, full_out );
  *(uint32_t*)out = full_out[0];
}

void Murmur3c_32a ( const void * key, const int len, uint32_t seed, void * out )
{
  uint32_t full_out[4];
  MurmurHash3_x86_128 ( key, len, seed, full_out );
  *(uint32_t*)out = full_out[1];
}

void Murmur3c_32b ( const void * key, const int len, uint32_t seed, void * out )
{
  uint32_t full_out[4];
  MurmurHash3_x86_128 ( key, len, seed, full_out );
  *(uint32_t*)out = full_out[2];
}

void Murmur3c_32c ( const void * key, const int len, uint32_t seed, void * out )
{
  uint32_t full_out[4];
  MurmurHash3_x86_128 ( key, len, seed, full_out );
  *(uint32_t*)out = full_out[3];
}
