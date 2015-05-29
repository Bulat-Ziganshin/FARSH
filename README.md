# FARSH
Fast and reliable (but not secure) 32-bit hash. Longer hashes (of `32*n` bits, up to 1024 bits) can be calculated by `farsh_n()` with n-fold speed loss. Main loop uses universal hashing formula from [UMAC](http://en.wikipedia.org/wiki/UMAC) with a precomputed key material of 1024 bytes (plus 128 bytes for longer hashes). You can use the FARSH as keyed hash by calling `farsh_keyed()` with 1024-byte key or `farsh_keyed_n()` with key of `1024+(n-1)*4` bytes.

NOTE: FARSH isn't yet ready for practical use since SMHasher shows a lot of problems in current implementation. But FARSH main loop implements universal hashing scheme that's mathematically proven to guarantee ideal hashing (as far as key material is random) and it employs the formula successfully used in cryptographic UMAC/VMAC algorithms.

FARSH is essentially UHASH with higher-level hashing algorithms replaced with simpler non-cryptographic ones. Current versions of FARSH high-level algorithms are very weak and this leads to hash weakness discovered by SMHasher, but I expect that improving these algorithms will lead to hash that's both faster and more secure than XXHash and MurMurHash. The ideas that can be tried here is tabulated hashing, CRC and the same algos from XXHash and MurMurHash.

Hence, I propose you to clone the repository and try to develop your own versions of the bit mixers, checking results with bundled SMHasher. Other areas of improvements you can do include reliable system for unrolling of main loop (since compilers are very weak in this area and MSVC/ICL were unable to unroll main loop), and streaming API - but this probably should be postponed until development of good bit mixing algorithms.

# Features / to-do list
- [x] hashes up to 1024 bits long (`farsh_n`)
- [x] hashes with user-supplied key material (`farsh_keyed` and `farsh_keyed_n`)
- [x] SSE2/AVX2 manually-optimized main loop
- [ ] manual unrolling of main loop (since msvc/icl can't do it themselves)
- [x] SMHasher testsuite
- [ ] fix issues found by SMHasher by tuning `COMPRESS_ULONG()` and hashsum combining
- [ ] `farsh_init/farsh_update/farsh_result` streaming API

# Universal hashing

The universal hashing formula used here is as simple as
```
    for (i=0; i < elements; i+=2)
        sum += (data[i] + key[i]) * (ULONG)(data[i+1] + key[i+1]);
```

## The main loop
- [Source code](farsh.c#L34)
- Asm code (found by adcl/pmuludq)
  - [gcc -O3 -funroll-loops -m32](gcc32.lst#L286)
  - [gcc -O3 -funroll-loops -m32 -msse2 -DSSE2](gcc32sse2.lst#L326)
  - [gcc -O3 -funroll-loops -m64 -msse2 -DSSE2](gcc64sse2.lst#L257)
  - [gcc -O3 -funroll-loops -m64 -mavx2 -DAVX2](gcc64avx2.lst#L259)

## Benchmark on Haswell i7-4770
```
C:\>gcc -O3 -funroll-loops -m32 main.cpp & a
6.331 GB/s = 5.896 GiB/s

C:\>gcc -O3 -funroll-loops -m32 -msse2 -DSSE2 main.cpp & a
31.207 GB/s = 29.063 GiB/s

C:\>gcc -O3 -funroll-loops -m64 -msse2 -DSSE2 main.cpp & a
29.277 GB/s = 27.266 GiB/s

C:\>gcc -O3 -funroll-loops -m64 -mavx2 -DAVX2 main.cpp & a
55.483 GB/s = 51.672 GiB/s
```
