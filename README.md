FARSH stands for Fast and Reliable (but not Secure) 32-bit Hash.
While established [new speed records](#benchmark)
and [successfully passed](SMHasher/reports/smhasher-farsh32-report.txt) the [SMHasher] testsuite,
it's not as reliable as the [competition](#competition).
[Discussion and additional benchmarks](http://encode.ru/threads/2213-FARSH-hashing-30-GB-s!).

# Features / to-do list
- [x] compute hashes up to 1024 bits long
- [x] hashing with user-supplied key material
- [x] [successfully passed](SMHasher/reports/smhasher-farsh32-report.txt) the [SMHasher] testsuite
- [ ] even faster and better quality hash mixing
- [x] SSE2/AVX2 manually-optimized main loop
- [x] 16-byte aligned key material and (optionally) input data for maximum speed on older CPUs
- [ ] manual unrolling of main loop (since msvc/icl can't do it themselves) or asm code
- [ ] try PSLLQ instead of PSHUFD in SSE2 code to improve speed on older CPUs
- [ ] `farsh_init/farsh_update/farsh_result` streaming API
- [ ] `farsh64*/farsh128*` APIs for faster computation of multi-word hashes
- [ ] `SSE2/AVX2/NEON?` options in the API (+alignment check for SSE2) for selection of the code path instead of compile-time choice


# [API](farsh.h)
- `uint32_t farsh(void *data, size_t size, uint64_t seed)`
returns 32-bit hash of the buffer
- `void farsh_n(void *data, size_t size, int k, int n, uint64_t seed, void *hash)`
computes `n` 32-bit hashes starting with the hash number `k`, storing results to the `hash` buffer.
It's `n` times slower than computation of single 32-bit hash.
Hash computed by the `farsh` function has number 0. The function aborts if `k+n > 32`.
- `uint32_t farsh_keyed(void *data, size_t size, void *key, uint64_t seed)`
computes 32-bit hash using `key`, that should be 1024-byte long and aligned to 16-byte boundary.
- `void farsh_keyed_n(void *data, size_t size, void *key, int n, uint64_t seed, void *hash)`
computes `n` 32-bit hashes using `key`, storing results to the `hash` buffer.
`key` should be `1024+16*(n-1)` bytes long and aligned to 16-byte boundary.
- Hash functions accept 64-bit `seed` that can be used to "personalize" the hash value. Use seed==0 if you don't need that feature.
Seeding may have lower quality than in the [competition](#competition) since the seed value mixed with block hashes rather than raw data.
- Header file provides symbolic names for the above-mentioned constants:
`FARSH_MAX_HASHES == 32,  FARSH_BASE_KEY_SIZE == 1024,  FARSH_BASE_KEY_ALIGNMENT == 16,  FARSH_EXTRA_KEY_SIZE == 16`


# Internals
The current FARSH version combines two hashing algorithms.

Low-level hashing algorithm splits all input data into 1024-byte blocks and computes hash value for every block.
It's the very short cycle borrowed from [UHASH] that combines 1024 bytes of input data with 1024 bytes of key material.
The hash value returned by this cycle is 64-bit long, and [UMAC thesis] proved that it has 32 bits of entropy.
So the low-level algorithm compresses each 1024-byte block of input data into 64-bit value carrying 32 bits of entropy.

High-level hashing algorithm is a stripped-down version of [xxHash64]. It receives sequence of 64-bit values from the previous level
and combines them into final 32-bit hash result. Since the original [xxHash64] algorithm successfully passes all [SMHasher] tests
while computing 64-bit hash from raw data, it's no surprise that modified algorithm is able to compute high-quality 32-bit hash
from the sequence of numbers each carrying 32 bits of entropy.

The power of the FARSH algorithm comes from its inner cycle, that is very short (read: fast) and allows highly-parallel implementations,
so it can fully exploit power of multi-core, SIMD, VLIW and SIMT (GPU) architectures.
At the same time, there is math proof that it can deliver 32 bits of entropy so we can use it without any doubts.


## Universal hashing
Main loop uses [universal hashing] formula from [UMAC] with a precomputed key material of 1024 bytes (plus 512 bytes for longer hashes).
FARSH is essentially [UHASH] with higher-level hashing algorithms replaced with simpler non-cryptographic one.
The universal hashing formula used here (and copied intact from UMAC) is as simple as
```C
    uint64_t sum = 0;  uint32_t *data, *key;
    for (i=0; i < elements; i+=2)
        sum  +=  uint64_t(data[i] + key[i]) * (data[i+1] + key[i+1]);
```

## The main loop
- [Source code](farsh.c#L28)
- Asm code (can be found by searching for adcl+mull/pmuludq instructions)
  - [gcc -O3 -funroll-loops -m32](asm-listings/gcc-x86.lst#L340)
  - [gcc -O3 -funroll-loops -m32 -msse2 -DFARSH_SSE2](asm-listings/gcc-x86-sse2.lst#L349)
  - [gcc -O3 -funroll-loops -m32 -mavx2 -DFARSH_AVX2](asm-listings/gcc-x86-avx2.lst#L350)
  - [gcc -O3 -funroll-loops -m64        -DFARSH_SSE2](asm-listings/gcc-x64.lst#L252)
  - [gcc -O3 -funroll-loops -m64 -mavx2 -DFARSH_AVX2](asm-listings/gcc-x64-avx2.lst#L259)


# Benchmark
Benchmark done on Haswell i7-4770 (3.9 GHz), [compiled](benchmark/compile.cmd) by GCC 4.9.2.
Only versions with 64-byte aligned input buffers were benchmarked.

Executable                |  FARSH 0.2 speed             |  Internal loop speed
--------------------------|-----------------------------:|----------------------------:
aligned-farsh-x64-avx2    |  54.536 GB/s = 50.790 GiB/s  |  65.645 GB/s = 61.137 GiB/s
aligned-farsh-x64         |  31.162 GB/s = 29.022 GiB/s  |  35.722 GB/s = 33.269 GiB/s
aligned-farsh-x86-avx2    |  40.279 GB/s = 37.513 GiB/s  |  61.682 GB/s = 57.446 GiB/s
aligned-farsh-x86-sse2    |  25.221 GB/s = 23.489 GiB/s  |  33.584 GB/s = 31.277 GiB/s
aligned-farsh-x86         |   6.255 GB/s =  5.825 GiB/s  |   6.336 GB/s =  5.901 GiB/s

Internal loop speed is a hard limit for the speed of any future FARSH version,
while version 0.2 speed includes time required for pretty slow high-level hashing.
Future versions should replace it with faster algorithm still satisfying the [SMHasher] requirements,
making overall hash speed within 10% of the internal loop speed.


# Competition
Fast non-cryptographic hashes:
- [HighwayHash](https://github.com/google/highwayhash) (2016)
- [xxHash][xxHash] (2012) and [xxHash64][xxHash64] (2014)
- The [CityHash](https://github.com/google/cityhash) family of hash functions (2011)
- [MurmurHash3](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp) (2011)
- [SpookyHash](http://burtleburtle.net/bob/hash/spooky.html): a 128-bit noncryptographic hash (2012)
- Interesting historical [overview](http://blog.reverberate.org/2012/01/state-of-hash-functions-2012.html)
- [More info](https://github.com/aappleby/smhasher/wiki/SMHasher) about the [SMHasher] testsuite

MAC/PRF, i.e. cryprographically secure keyed hashes:
- [UMAC] and [VMAC]
- The [Poly1305-AES](https://en.wikipedia.org/wiki/Poly1305) message-authentication code
- [SipHash](https://131002.net/siphash/)
- Cryptoanalysis of [CityHash64, MurmurHash](https://131002.net/siphash/#at) and [xxHash](http://crypto.stackexchange.com/questions/6408/from-hash-to-cryptographic-hash)



[VMAC]: http://en.wikipedia.org/wiki/VMAC
[UMAC]: http://en.wikipedia.org/wiki/UMAC
[UMAC thesis]: http://fastcrypto.org/umac/umac_thesis.pdf
[UHASH]: https://tools.ietf.org/html/rfc4418#section-5
[universal hashing]: http://en.wikipedia.org/wiki/Universal_hashing
[xxHash]: https://github.com/Cyan4973/xxHash
[xxHash64]: https://github.com/Cyan4973/xxHash
[SMHasher]: https://github.com/aappleby/smhasher
