# FARSH
Fast and reliable (but not secure) 32-bit hash. Longer hashes (of `32*N` bits, up to 1024 bits) can be calculated by `farsh_n()` with N-fold speed loss. Main loop uses [universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) formula from [UMAC](http://en.wikipedia.org/wiki/UMAC) with a precomputed key material of 1024 bytes (plus 512 bytes for longer hashes). Also you can use the FARSH as 32-bit keyed hash by calling `farsh_keyed()` with 1024-byte key or as `32*N -bit` keyed hash by calling `farsh_keyed_n()` with key of `1008+N*16` bytes. All FARSH hashing functions also accept 64-bit `seed` value.

# Features / to-do list
- [x] hashes up to 1024 bits long (`farsh_n`)
- [x] hashes with user-supplied key material (`farsh_keyed` and `farsh_keyed_n`)
- [x] [successfully passed](SMHasher/reports/smhasher-farsh32-report.txt) the [SMHasher](https://code.google.com/p/smhasher) testsuite
- [ ] even faster and better quality hash mixing
- [x] SSE2/AVX2 manually-optimized main loop
- [x] 16-byte aligned key material and (optionally) input data for maximum speed on older CPUs
- [ ] manual unrolling of main loop (since msvc/icl can't do it themselves)
- [ ] try PSLLQ instead of PSHUFD in SSE2 code
- [ ] `farsh_init/farsh_update/farsh_result` streaming API

# Internals
FARSH is essentially [UHASH](https://tools.ietf.org/html/rfc4418#section-5) with higher-level hashing algorithms replaced by simpler non-cryptographic ones. [Universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) kernel derived from UHASH returns 64-bit hash having 32-bit entropy for each successive 1024-byte block of input data, and higher-level hash combining code derived from xxHash64 mix block hashes. 

We can try to further improve the hash combining by employing tabulated hashing, CRC and formulas from XXHash and MurMurHash.

OLD NOTE: FARSH isn't yet ready for practical use since SMHasher shows [a lot of problems](SMHasher/reports/smhasher-farsh32-report.txt) in the current implementation. But FARSH main loop implements [universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) scheme that's mathematically proven to guarantee ideal hashing (as far as key material is random) and it employs the formula successfully used in cryptographic [UMAC](http://en.wikipedia.org/wiki/UMAC)/[VMAC](http://en.wikipedia.org/wiki/VMAC) algorithms.

FARSH is essentially [UHASH](https://tools.ietf.org/html/rfc4418#section-5) with higher-level hashing algorithms replaced by simpler non-cryptographic ones. Current versions of FARSH high-level algorithms are very weak and this leads to hash weakness discovered by [SMHasher](https://code.google.com/p/smhasher), but I expect that improving these algorithms will lead to hash that's both faster and more secure than [XXHash](https://github.com/Cyan4973/xxHash) and [MurMurHash](http://en.wikipedia.org/wiki/MurmurHash). The ideas that can be tried here is tabulated hashing, CRC and the same algos from XXHash and MurMurHash.

Hence, I propose you to clone the repository and try to develop your own versions of the bit mixers, checking results with bundled [SMHasher](https://code.google.com/p/smhasher). Other areas of improvements you can do include reliable system for unrolling of main loop (since compilers are very weak in this area and MSVC/ICL were unable to unroll main loop), and streaming API - but this probably should be postponed until development of good bit mixing algorithms.

# Universal hashing
The [universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) formula used here (and derived from UMAC) is as simple as
```C
    for (i=0; i < elements; i+=2)
        sum += (data[i] + key[i]) * (ULONG)(data[i+1] + key[i+1]);
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
