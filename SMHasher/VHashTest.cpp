#include "VMAC/vmac.c"

unsigned char vhash_key[] = "123456789abcdef";

void vhash64_test ( const void * key, int len, unsigned seed, void * out )
{
  static vmac_ctx_t ctx[1];  static int inited=0;
  if (!inited)
    inited=1, vmac_set_key(vhash_key, ctx);

  *(uint64_t*) out = vhash((unsigned char*)key, len, NULL, ctx);
}
