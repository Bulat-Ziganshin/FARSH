#include <stdint.h>
#include "xxHash/xxhash.c"

void XXH32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = XXH32(key,len,seed);
}


/* ***********************************************
*  New hash functions developed by Bulat Ziganshin
*************************************************/

static U32 MyXXH32_round(U32 h1, U32 h2, U32 input)
{
    h1 += input;
    h1 *= PRIME32_1;
    h1 = XXH_rotl32(h1, 13);
    h1 += h2;
    h1 *= PRIME32_2;
//    h1 = XXH_rotl32(h1, 13);
    return h1;
}

FORCE_INLINE U32 MyXXH32(const void* input, size_t len, U32 seed)
{
    const BYTE* p = (const BYTE*)input;
    const BYTE* bEnd = p + len;
    U32 h32;
#undef XXH_get32bits
#define XXH_get32bits(p) (*(uint32_t*)(p))

    if (len>=16) {
        const BYTE* const limit = bEnd - 16;
        U32 v1 = seed + PRIME32_1 + PRIME32_2;
        U32 v2 = seed + PRIME32_2;
        U32 v3 = seed + 0;
        U32 v4 = seed - PRIME32_1;

        do {
            v1 = MyXXH32_round(v1, v2, XXH_get32bits(p)); p+=4;
            v2 = MyXXH32_round(v2, v3, XXH_get32bits(p)); p+=4;
            v3 = MyXXH32_round(v3, v4, XXH_get32bits(p)); p+=4;
            v4 = MyXXH32_round(v4, v1, XXH_get32bits(p)); p+=4;
        } while (p<=limit);

        h32 = XXH_rotl32(v1, 1) + XXH_rotl32(v2, 7) + XXH_rotl32(v3, 12) + XXH_rotl32(v4, 18);
    } else {
        h32  = seed + PRIME32_5;
    }

    h32 += (U32) len;

    while (p+4<=bEnd) {
        h32 += XXH_get32bits(p) * PRIME32_3;
        h32  = XXH_rotl32(h32, 17) * PRIME32_4 ;
        p+=4;
    }

    while (p<bEnd) {
        h32 += (*p) * PRIME32_5;
        h32 = XXH_rotl32(h32, 11) * PRIME32_1 ;
        p++;
    }

    h32 ^= h32 >> 15;
    h32 *= PRIME32_2;
    h32 ^= h32 >> 13;
    h32 *= PRIME32_3;
    h32 ^= h32 >> 16;

    return h32;
}

void MyXXH32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = MyXXH32(key,len,seed);
}
