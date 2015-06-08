FARSH - Fast and [Reliable](SMHasher/reports/smhasher-farsh32-report.txt) (but not Secure) 32-bit Hash. More info at [encode.ru](http://encode.ru/threads/2213-FARSH-hashing-30-GB-s!)

# Features / to-do list
- [x] compute hashes up to 1024 bits long
- [x] hashing with user-supplied key material
- [x] [successfully passed](SMHasher/reports/smhasher-farsh32-report.txt) the [SMHasher](https://code.google.com/p/smhasher) testsuite
- [ ] even faster and better quality hash mixing
- [x] SSE2/AVX2 manually-optimized main loop
- [x] 16-byte aligned key material and (optionally) input data for maximum speed on older CPUs
- [ ] manual unrolling of main loop (since msvc/icl can't do it themselves) or asm code
- [ ] try PSLLQ instead of PSHUFD in SSE2 code to improve speed on older CPUs
- [ ] `farsh_init/farsh_update/farsh_result` streaming API
- [ ] `farsh64*/farsh128*` APIs for faster computation of multi-word hashes
 
#API
- `U32 farsh(void *data, size_t size, U64 seed)` returns 32-bit hash of the `data`
- `void farsh_n(void *data, size_t size, int k, int n, U64 seed, void *hash)` computes `n` 32-bit hashes starting with the hash number `k` and writes the results to the `hash`. Hash computed by `farsh` has number 0. The function aborts if `n+k>32`.
- `U32 farsh_keyed(void *data, size_t size, void *key, U64 seed)` computes 32-bit hash using 1024-byte long 16-byte aligned `key`.
- `void farsh_keyed_n(void *data, size_t size, void *key, int n, U64 seed, void *hash)` computes `n` 32-bit hashes using `1008+n*16`-byte long 16-byte aligned `key` and writes the results to the `hash`.
- Every hash function accepts 64-bit `seed` that can be used to "personalize" the hash value. Seeding may be lower-quality than in the MurMurHash/xxHash because seed value is mixed with block hashes rather than raw data.

# Internals
The current FARSH version is built from two hashing algorithms. 

Lower-level hash algorithm splits all input data into 1024-byte blocks and computes hash value for every block. It's the very simple cycle borrowed from UHASH that combines 1024 bytes of input data with 1024 bytes of key material. The hash value returned by this cycle is 64-bit long, and there is a math proof that it has 32 bits of entropy. So the lower-level algorithm compresses 1024-byte blocks of input data into 64-bit values each carrying 32 bits of entropy. 

Higher-level hash algorithm is stripped-down version of xxHash64. It gets sequence of 64-bit values from the previous level and combines them into 32-bit hash value. Since the original xxHash64 algorithm successfully passes all SMHasher tests while computing 64-bit hash from raw data, it's no surprise that modified algorithm is able to compute high-quality 32-bit hash from sequence of numbers with 32-bit entropy.






# Scratchpad
Longer hashes (of `32*N` bits, up to 1024 bits) can be calculated by `farsh_n()` with N-fold speed loss. Main loop uses [universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) formula from [UMAC](http://en.wikipedia.org/wiki/UMAC) with a precomputed key material of 1024 bytes (plus 512 bytes for longer hashes). Also you can use the FARSH as 32-bit keyed hash by calling `farsh_keyed()` with 1024-byte key or as `32*N -bit` keyed hash by calling `farsh_keyed_n()` with key of `1008+N*16` bytes. All FARSH hashing functions also accept 64-bit `seed` value.

FARSH is essentially [UHASH](https://tools.ietf.org/html/rfc4418#section-5) with higher-level hashing algorithms replaced by simpler non-cryptographic ones. [Universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) kernel derived from UHASH returns 64-bit hash having 32-bit entropy for each successive 1024-byte block of input data, and higher-level hash combining code derived from xxHash64 mix block hashes. 

We can try to further improve the hash combining by employing tabulated hashing, CRC and formulas from XXHash and MurMurHash.

OLD NOTE: FARSH isn't yet ready for practical use since SMHasher shows [a lot of problems](SMHasher/reports/smhasher-farsh32-report.txt) in the current implementation. But FARSH main loop implements [universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) scheme that's mathematically proven to guarantee ideal hashing (as far as key material is random) and it employs the formula successfully used in cryptographic [UMAC](http://en.wikipedia.org/wiki/UMAC)/[VMAC](http://en.wikipedia.org/wiki/VMAC) algorithms.

FARSH is essentially [UHASH](https://tools.ietf.org/html/rfc4418#section-5) with higher-level hashing algorithms replaced by simpler non-cryptographic ones. Current versions of FARSH high-level algorithms are very weak and this leads to hash weakness discovered by [SMHasher](https://code.google.com/p/smhasher), but I expect that improving these algorithms will lead to hash that's both faster and more secure than [XXHash](https://github.com/Cyan4973/xxHash) and [MurMurHash](http://en.wikipedia.org/wiki/MurmurHash). The ideas that can be tried here is tabulated hashing, CRC and the same algos from XXHash and MurMurHash.

Hence, I propose you to clone the repository and try to develop your own versions of the bit mixers, checking results with bundled [SMHasher](https://code.google.com/p/smhasher). Other areas of improvements you can do include reliable system for unrolling of main loop (since compilers are very weak in this area and MSVC/ICL were unable to unroll main loop), and streaming API - but this probably should be postponed until development of good bit mixing algorithms.

# Universal hashing
The [universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) formula used here (and derived from UMAC) is as simple as
```C
    uint64 sum = 0;  uint32 *data, *key;
    for (i=0; i < elements; i+=2)
        sum += (uint64)(data[i] + key[i]) * (uint64)(data[i+1] + key[i+1]);
```

## The main loop
- [Source code](farsh.c#L22)
- Asm code (found by searching for adcl+mull/pmuludq instructions)
  - [gcc -O3 -funroll-loops -m32](asm-listings/gcc-x86.lst#L300)
  - [gcc -O3 -funroll-loops -m32 -msse2 -DSSE2](asm-listings/gcc-x86-sse2.lst#L323)
  - [gcc -O3 -funroll-loops -m32 -mavx2 -DAVX2](asm-listings/gcc-x86-avx2.lst#L320)
  - [gcc -O3 -funroll-loops -m64 -DSSE2](asm-listings/gcc-x64.lst#L260)
  - [gcc -O3 -funroll-loops -m64 -mavx2 -DAVX2](asm-listings/gcc-x64-avx2.lst#L262)

## Benchmark on Haswell i7-4770
Executable      | Speed                       | Compiler
----------------|-----------------------------|---------
farsh-x86       |  6.457 GB/s =  6.014 GiB/s  |gcc -O3 -funroll-loops -m32
farsh-x86-sse2  | 30.436 GB/s = 28.346 GiB/s  |gcc -O3 -funroll-loops -m32 -msse2 -DSSE2
farsh-x86-avx2  | 58.783 GB/s = 54.746 GiB/s  |gcc -O3 -funroll-loops -m32 -mavx2 -DAVX2
farsh-x64       | 26.296 GB/s = 24.490 GiB/s  |gcc -O3 -funroll-loops -m64 -DSSE2
farsh-x64-avx2  | 57.904 GB/s = 53.927 GiB/s  |gcc -O3 -funroll-loops -m64 -mavx2 -DAVX2
