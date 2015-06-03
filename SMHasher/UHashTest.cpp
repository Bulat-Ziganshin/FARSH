#include "UMAC/umac.c"
#include "UMAC/rijndael-alg-fst.c"

char uhash_key[] = "123456789abcdef";
uhash_ctx_t ahc = uhash_alloc(uhash_key);

void uhash32_test ( const void * key, int len, unsigned seed, void * out )
{
  uhash_reset (ahc);
  uhash_update (ahc, (char*)key, len);
  uhash_final (ahc, (char*)out);
}
