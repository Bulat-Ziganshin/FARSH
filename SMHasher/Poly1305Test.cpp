#include "poly1305/poly1305.c"

const unsigned int poly1305_key[POLY1305_KEYLEN/4] = {
  // 16-byte AES key k
  0xb8fe6c39,0x23a44bbe,0x7c01812c,0xf721ad1c,
  // r[3], r[7], r[11], r[15] are required to have their top four bits clear, and r[4], r[8], r[12] are required to have their bottom two bits clear
  0xed46de8, 0x39097d8, 0x240a4a4, 0x7b3671c};

void poly1305_test ( const void * key, int len, unsigned seed, void * out )
{
  poly1305_auth ((unsigned char*)out, (const unsigned char*)key, len, (const unsigned char*)poly1305_key);
}
