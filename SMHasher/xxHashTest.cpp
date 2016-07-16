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
*  XXH32 with XXH64 finalization
*************************************************/

#undef XXH_get32bits
#define XXH_get32bits(p) (*(uint32_t*)(p))

typedef U32 update_xxf (U32 h1, U32 input);

FORCE_INLINE U64 GenericXXHash (update_xxf update, const void* input, size_t len, U32 seed)
{
    const BYTE* p = (const BYTE*)input;
    const BYTE* bEnd = p + len;
#define ROUND4()                             \
{                                            \
    v1 = update(v1, XXH_get32bits(p)); p+=4; \
    v2 = update(v2, XXH_get32bits(p)); p+=4; \
    v3 = update(v3, XXH_get32bits(p)); p+=4; \
    v4 = update(v4, XXH_get32bits(p)); p+=4; \
}

    U32 v1 = seed + PRIME32_1 + PRIME32_2;
    U32 v2 = seed + PRIME32_2;
    U32 v3 = seed + 0;
    U32 v4 = seed - PRIME32_1;

    if (len>=16) {
        const BYTE* const limit = bEnd - 16;
        do {
            ROUND4();
        } while (p<=limit);
    }

    U32 remainder[4] = {0};
    memcpy(remainder, p, bEnd-p);
    p = (const BYTE*)remainder;
    ROUND4();

    U64 h64 = U64(v1) + (U64(v2) << 11) + (U64(v3) << 21) + (U64(v4) << 32);
    h64 = XXH64_mergeRound(h64, (U32) len);
    h64 = XXH64_mergeRound(h64, v1);
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


void ModXXH32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericXXHash (XXH32_round,key,len,seed);
}

void ModXXH32a_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericXXHash (XXH32_round,key,len,seed) >> 32;
}

void ModXXH64_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint64_t*)out = GenericXXHash (XXH32_round,key,len,seed);
}


/* ***********************************************
*  New hash functions developed by Bulat Ziganshin
*************************************************/

typedef U32 update_f (U32 h1, U32 h2, U32 input);

FORCE_INLINE U64 GenericHash (update_f update, const void* input, size_t len, U32 seed)
{
    const BYTE* p = (const BYTE*)input;
    const BYTE* bEnd = p + len;
#define ROUND()                                  \
{                                                \
    v1 = update(v1, v2, XXH_get32bits(p)); p+=4; \
    v2 = update(v2, v3, XXH_get32bits(p)); p+=4; \
    v3 = update(v3, v4, XXH_get32bits(p)); p+=4; \
    v4 = update(v4, v5, XXH_get32bits(p)); p+=4; \
    v5 = update(v5, v1, XXH_get32bits(p)); p+=4; \
}

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
    h64 = XXH64_mergeRound(h64, (U32) len);
    h64 = XXH64_mergeRound(h64, v1);
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
    h1 += input;
    h1 *= PRIME32_1;
    h1 += h2;
    h1 = XXH_rotl32(h1, 13);
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


/* ***************************************************
*  New x64 hash functions developed by Bulat Ziganshin
*****************************************************/

#undef XXH_get64bits
#define XXH_get64bits(p) (*(uint64_t*)(p))

typedef U64 update64_f (U64 h1, U64 h2, U64 input);

FORCE_INLINE U64 GenericWideHash (update64_f update, const void* input, size_t len, U64 seed)
{
    const BYTE* p = (const BYTE*)input;
    const BYTE* bEnd = p + len;
#define WideROUND()                                 \
{                                                   \
    v1  = update(v1 , v2 , XXH_get64bits(p)); p+=8; \
    v2  = update(v2 , v3 , XXH_get64bits(p)); p+=8; \
    v3  = update(v3 , v4 , XXH_get64bits(p)); p+=8; \
    v4  = update(v4 , v5 , XXH_get64bits(p)); p+=8; \
    v5  = update(v5 , v6 , XXH_get64bits(p)); p+=8; \
    v6  = update(v6 , v7 , XXH_get64bits(p)); p+=8; \
    v7  = update(v7 , v8 , XXH_get64bits(p)); p+=8; \
    v8  = update(v8 , v9 , XXH_get64bits(p)); p+=8; \
    v9  = update(v9 , v10, XXH_get64bits(p)); p+=8; \
    v10 = update(v10, v11, XXH_get64bits(p)); p+=8; \
    v11 = update(v11, v12, XXH_get64bits(p)); p+=8; \
    v12 = update(v12, v1 , XXH_get64bits(p)); p+=8; \
}

    U64 v1  = seed + PRIME64_1 + PRIME64_2;
    U64 v2  = seed + PRIME64_1 + PRIME64_3;
    U64 v3  = seed + PRIME64_1 + PRIME64_4;
    U64 v4  = seed + PRIME64_1 - PRIME64_2;
    U64 v5  = seed + PRIME64_1 - PRIME64_3;
    U64 v6  = seed + PRIME64_1 - PRIME64_4;
    U64 v7  = seed - PRIME64_1 + PRIME64_2;
    U64 v8  = seed - PRIME64_1 + PRIME64_3;
    U64 v9  = seed - PRIME64_1 + PRIME64_4;
    U64 v10 = seed - PRIME64_1 - PRIME64_2;
    U64 v11 = seed - PRIME64_1 - PRIME64_3;
    U64 v12 = seed - PRIME64_1 - PRIME64_4;

    if (len>=8*12) {
        const BYTE* const limit = bEnd - 8*12;
        do {
            WideROUND();
        } while (p<=limit);
    }

    U64 remainder[12] = {0};
    memcpy(remainder, p, bEnd-p);
    p = (const BYTE*)remainder;
    WideROUND();

    U64 h64 = XXH_rotl64(v1, 1) + XXH_rotl64(v2, 7) + XXH_rotl64(v3, 12) + XXH_rotl64(v4, 18);
    h64 = XXH64_mergeRound(h64, (U64) len);
    h64 = XXH64_mergeRound(h64, v9 );
    h64 = XXH64_mergeRound(h64, v10);
    h64 = XXH64_mergeRound(h64, v11);
    h64 = XXH64_mergeRound(h64, v12);

    h64 += XXH_rotl64(v5, 1) + XXH_rotl64(v6, 7) + XXH_rotl64(v7, 12) + XXH_rotl64(v8, 18);
    h64 = XXH64_mergeRound(h64, v1 );
    h64 = XXH64_mergeRound(h64, v2 );
    h64 = XXH64_mergeRound(h64, v3 );
    h64 = XXH64_mergeRound(h64, v4 );

    h64 += XXH_rotl64(v9, 1) + XXH_rotl64(v10,7) + XXH_rotl64(v11,12) + XXH_rotl64(v12,18);
    h64 = XXH64_mergeRound(h64, v5 );
    h64 = XXH64_mergeRound(h64, v6 );
    h64 = XXH64_mergeRound(h64, v7 );
    h64 = XXH64_mergeRound(h64, v8 );

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

static U64 WideZZH64_round (U64 h1, U64 h2, U64 input)
{
    h1 += input;
    h1 *= PRIME64_1;
    h1 += h2;
    h1 = XXH_rotl64(h1, 13);
    return h1;
}

void WideZZH32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericWideHash (WideZZH64_round,key,len,seed);
}

void WideZZH32a_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericWideHash (WideZZH64_round,key,len,seed) >> 32;
}

void WideZZH64_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint64_t*)out = GenericWideHash (WideZZH64_round,key,len,seed);
}


/* ***********************************************
*  Slow ZZ-hashes
*************************************************/

static U64 SlowWideZZH64_round (U64 h1, U64 h2, U64 input)
{
    h1 += input;
    h1 *= PRIME64_1;
    h1 = XXH_swap64(h1);
    h1 += h2;
    h1 *= PRIME64_2;
//    h1 = XXH_rotl64(h1, 13);
    return h1;
}

void SlowWideZZH32_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericWideHash (SlowWideZZH64_round,key,len,seed);
}

void SlowWideZZH32a_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint32_t*)out = GenericWideHash (SlowWideZZH64_round,key,len,seed) >> 32;
}

void SlowWideZZH64_test ( const void * key, int len, unsigned seed, void * out )
{
  *(uint64_t*)out = GenericWideHash (SlowWideZZH64_round,key,len,seed);
}
