#include "VMAC/vmac.c"

unsigned char vhash_key1[] = "0123456789abcdef";
unsigned char vhash_key2[] = "fedcba9876543210";

void vhash64_test ( const void * key, int len, unsigned seed, void * out )
{
  static vmac_ctx_t ctx[1];  static int inited=0;
  if (!inited)
    inited=1, vmac_set_key(vhash_key1, ctx);

  *(uint64_t*) out = vhash((unsigned char*)key, len, NULL, ctx);
}

void vhash128_test ( const void * key, int len, unsigned seed, void * out )
{
  static vmac_ctx_t ctx[2];  static int inited=0;
  if (!inited)
    inited=1, vmac_set_key(vhash_key1, ctx), vmac_set_key(vhash_key2, ctx+1);

  ((uint64_t*)out)[0] = vhash((unsigned char*)key, len, NULL, ctx);
  ((uint64_t*)out)[1] = vhash((unsigned char*)key, len, NULL, ctx+1);
}
