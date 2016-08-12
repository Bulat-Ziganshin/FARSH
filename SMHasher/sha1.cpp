// Test 32/64/128-bit parts of SHA1

#include "Hashes.h"
#include "SHA1/sha1.cpp"

void sha1 ( const void * key, int len, uint32_t seed, void * out, int start, int count )
{
  SHA1_CTX context;

  uint8_t digest[20], seed8[] = {seed, seed>>8, seed>>16, seed>>24};

  SHA1_Init(&context);
  SHA1_Update(&context, seed8, 4);
  SHA1_Update(&context, (uint8_t*)key, len);
  SHA1_Final(&context, digest);
 
  memcpy(out, &digest[start*4], count*4);
}

void sha1_32  ( const void * key, int len, uint32_t seed, void * out )   {sha1(key,len,seed,out,0,1);}
void sha1_32a ( const void * key, int len, uint32_t seed, void * out )   {sha1(key,len,seed,out,1,1);}
void sha1_32b ( const void * key, int len, uint32_t seed, void * out )   {sha1(key,len,seed,out,2,1);}
void sha1_32c ( const void * key, int len, uint32_t seed, void * out )   {sha1(key,len,seed,out,3,1);}
void sha1_64  ( const void * key, int len, uint32_t seed, void * out )   {sha1(key,len,seed,out,0,2);}
void sha1_64a ( const void * key, int len, uint32_t seed, void * out )   {sha1(key,len,seed,out,2,2);}
void sha1_128 ( const void * key, int len, uint32_t seed, void * out )   {sha1(key,len,seed,out,0,4);}
