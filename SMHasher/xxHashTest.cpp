#include <stdint.h>
#include "xxHash/xxhash.c"

void XXH32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = XXH32(key,len,seed);
}

void XXH64_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint64_t*)out = XXH64(key,len,seed);
}


/* ***********************************************
*  New hash functions developed by Bulat Ziganshin
*************************************************/

typedef U32 update_f (U32 h1, U32 h2, U32 input);

#define ROUND()                                  \
{                                                \
    v1 = update(v1, v2, XXH_get32bits(p)); p+=4; \
    v2 = update(v2, v3, XXH_get32bits(p)); p+=4; \
    v3 = update(v3, v4, XXH_get32bits(p)); p+=4; \
    v4 = update(v4, v5, XXH_get32bits(p)); p+=4; \
    v5 = update(v5, v1, XXH_get32bits(p)); p+=4; \
}

FORCE_INLINE U64 GenericHash (update_f update, const void* input, size_t len, U32 seed)
{
    const BYTE* p = (const BYTE*)input;
    const BYTE* bEnd = p + len;
#undef XXH_get32bits
#define XXH_get32bits(p) (*(uint32_t*)(p))

    U32 v1 = seed + PRIME32_1 + PRIME32_2;
    U32 v2 = seed + PRIME32_2;
    U32 v3 = seed + 0;
    U32 v4 = seed - PRIME32_1;
    U32 v5 = seed + PRIME32_3;

    if (len>=20) {
        const BYTE* const limit = bEnd - 20;
        do {
            ROUND();
        } while (p<=limit);
    }

    U32 remainder[5] = {0};
    memcpy(remainder, p, bEnd-p);
    p = (const BYTE*)remainder;
    ROUND();

    U64 h64 = U64(v1) + (U64(v2) << 8) + (U64(v3) << 16) + (U64(v4) << 24) + (U64(v5) << 32);
    h64 = XXH64_mergeRound(h64, v1 + (U32) len);
    h64 = XXH64_mergeRound(h64, v2);
    h64 = XXH64_mergeRound(h64, v3);
    h64 = XXH64_mergeRound(h64, v4);
    h64 = XXH64_mergeRound(h64, v5);

    h64 ^= h64 >> 33;
    h64 *= PRIME64_2;
    h64 ^= h64 >> 29;
    h64 *= PRIME64_3;
    h64 ^= h64 >> 32;

    return h64;
}


/* ***********************************************
*  Fast ZZ-hashes
*************************************************/

static U32 ZZH32_round (U32 h1, U32 h2, U32 input)
{
    h1 ^= input;
    h1 *= PRIME32_1;
    h1 += h2;
    h1 = XXH_swap32(h1);
    return h1;
}

void ZZH32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericHash (ZZH32_round,key,len,seed);
}

void ZZH32a_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericHash (ZZH32_round,key,len,seed) >> 32;
}

void ZZH64_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint64_t*)out = GenericHash (ZZH32_round,key,len,seed);
}


/* ***********************************************
*  Slow ZZ-hashes
*************************************************/

static U32 SlowZZH32_round (U32 h1, U32 h2, U32 input)
{
    h1 += input;
    h1 *= PRIME32_1;
    h1 = XXH_swap32(h1);
    h1 += h2;
    h1 *= PRIME32_2;
//    h1 = XXH_rotl32(h1, 13);
    return h1;
}

void SlowZZH32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericHash (SlowZZH32_round,key,len,seed);
}

void SlowZZH32a_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericHash (SlowZZH32_round,key,len,seed) >> 32;
}

void SlowZZH64_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint64_t*)out = GenericHash (SlowZZH32_round,key,len,seed);
}
