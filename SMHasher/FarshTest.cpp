#include "../farsh.c"

void farsh32_test ( const void * key, int len, unsigned seed, void * out )
{
  farsh_n(key,len,0,1,seed,out);
}

void farsh64_test ( const void * key, int len, unsigned seed, void * out )
{
  farsh_n(key,len,0,2,seed,out);
}

void farsh128_test ( const void * key, int len, unsigned seed, void * out )
{
  farsh_n(key,len,0,4,seed,out);
}

void farsh256_test ( const void * key, int len, unsigned seed, void * out )
{
  farsh_n(key,len,0,8,seed,out);
}
