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



FORCE_INLINE U64 MyXXH64(const void* input, size_t len, U32 seed)
{
    const BYTE* p = (const BYTE*)input;
    const BYTE* bEnd = p + len;
    U32 h32;
    U64 h64;
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

        h64 = U64(v1) + (U64(v2) << 11) + (U64(v3) << 21) + (U64(v4) << 32);
        h64 = XXH64_mergeRound(h64, v1);
        h64 = XXH64_mergeRound(h64, v2);
        h64 = XXH64_mergeRound(h64, v3);
        h64 = XXH64_mergeRound(h64, v4);
    } else {
        h64  = seed + PRIME64_5;
    }

    h64 += (U64) len;

    if (p+4<=bEnd) {
        h64 ^= (U64)(XXH_get32bits(p)) * PRIME64_1;
        h64 = XXH_rotl64(h64, 23) * PRIME64_2 + PRIME64_3;
        p+=4;
    }

    while (p<bEnd) {
        h64 ^= (*p) * PRIME64_5;
        h64 = XXH_rotl64(h64, 11) * PRIME64_1;
        p++;
    }

    h64 ^= h64 >> 33;
    h64 *= PRIME64_2;
    h64 ^= h64 >> 29;
    h64 *= PRIME64_3;
    h64 ^= h64 >> 32;

    return h64;
}

void MyXXH64_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint64_t*)out = MyXXH64(key,len,seed);
}


/* ***********************************************
*  Newest hash functions developed by Bulat Ziganshin
*************************************************/

static U32 ZZH32_round(U32 h1, U32 h2, U32 input)
{
    h1 ^= input;
    h1 += h2;
    h1 *= PRIME32_1;
    h1 = XXH_rotl32(h1, 13);
    return h1;
}

#define ROUND()                                       \
{                                                     \
    v1 = ZZH32_round(v1, v2, XXH_get32bits(p)); p+=4; \
    v2 = ZZH32_round(v2, v3, XXH_get32bits(p)); p+=4; \
    v3 = ZZH32_round(v3, v4, XXH_get32bits(p)); p+=4; \
    v4 = ZZH32_round(v4, v1, XXH_get32bits(p)); p+=4; \
}

FORCE_INLINE U64 ZZH64(const void* input, size_t len, U32 seed)
{
    const BYTE* p = (const BYTE*)input;
    const BYTE* bEnd = p + len;
#undef XXH_get32bits
#define XXH_get32bits(p) (*(uint32_t*)(p))

    U32 v1 = seed + PRIME32_1 + PRIME32_2;
    U32 v2 = seed + PRIME32_2;
    U32 v3 = seed + 0;
    U32 v4 = seed - PRIME32_1;

    if (len>=16) {
        const BYTE* const limit = bEnd - 16;
        do {
            ROUND();
        } while (p<=limit);
    }

    U32 remainder[4] = {0};
    memcpy(remainder, p, bEnd-p);
    p = (const BYTE*)remainder;
    ROUND();

    U64 h64 = U64(v1) + (U64(v2) << 11) + (U64(v3) << 21) + (U64(v4) << 32);
    h64 = XXH64_mergeRound(h64, v1 + (U32) len);
    h64 = XXH64_mergeRound(h64, v2);
    h64 = XXH64_mergeRound(h64, v3);
    h64 = XXH64_mergeRound(h64, v4);

    h64 ^= h64 >> 33;
    h64 *= PRIME64_2;
    h64 ^= h64 >> 29;
    h64 *= PRIME64_3;
    h64 ^= h64 >> 32;

    return h64;
}

void ZZH32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = ZZH64(key,len,seed);
}

void ZZH64_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint64_t*)out = ZZH64(key,len,seed);
}
