# FARSH
Fast and reliable (but not secure) 32-bit hash. Longer hashes (of `32*n` bits, up to 1024 bits) can be calculated by `farsh_n()` with n-fold speed loss. Main loop uses [universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) formula from [UMAC](http://en.wikipedia.org/wiki/UMAC) with a precomputed key material of 1024 bytes (plus 128 bytes for longer hashes). You can use the FARSH as keyed hash by calling `farsh_keyed()` with 1024-byte key or `farsh_keyed_n()` with key of `1024+(n-1)*4` bytes.

NOTE: FARSH isn't yet ready for practical use since SMHasher shows a lot of problems in current implementation. But FARSH main loop implements [universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) scheme that's mathematically proven to guarantee ideal hashing (as far as key material is random) and it employs the formula successfully used in cryptographic [UMAC](http://en.wikipedia.org/wiki/UMAC)/[VMAC](http://en.wikipedia.org/wiki/VMAC) algorithms.

FARSH is essentially [UHASH](https://tools.ietf.org/html/rfc4418#section-5) with higher-level hashing algorithms replaced with simpler non-cryptographic ones. Current versions of FARSH high-level algorithms are very weak and this leads to hash weakness discovered by [SMHasher](https://code.google.com/p/smhasher), but I expect that improving these algorithms will lead to hash that's both faster and more secure than [XXHash](https://github.com/Cyan4973/xxHash) and [MurMurHash](http://en.wikipedia.org/wiki/MurmurHash). The ideas that can be tried here is tabulated hashing, CRC and the same algos from XXHash and MurMurHash.

Hence, I propose you to clone the repository and try to develop your own versions of the bit mixers, checking results with bundled [SMHasher](https://code.google.com/p/smhasher). Other areas of improvements you can do include reliable system for unrolling of main loop (since compilers are very weak in this area and MSVC/ICL were unable to unroll main loop), and streaming API - but this probably should be postponed until development of good bit mixing algorithms.

# Features / to-do list
- [x] hashes up to 1024 bits long (`farsh_n`)
- [x] hashes with user-supplied key material (`farsh_keyed` and `farsh_keyed_n`)
- [x] SSE2/AVX2 manually-optimized main loop
- [ ] manual unrolling of main loop (since msvc/icl can't do it themselves)
- [x] [SMHasher](https://code.google.com/p/smhasher) testsuite
- [ ] fix issues found by [SMHasher](https://code.google.com/p/smhasher) by tuning `COMPRESS_ULONG()` and hashsum combining
- [ ] `farsh_init/farsh_update/farsh_result` streaming API

# Universal hashing

The [universal hashing](http://en.wikipedia.org/wiki/Universal_hashing) formula used here (and derived from UMAC) is as simple as
```C
    for (i=0; i < elements; i+=2)
        sum += (data[i] + key[i]) * (ULONG)(data[i+1] + key[i+1]);
```

## The main loop
- [Source code](farsh.c#L34)
- Asm code (found by searching for adcl+mull/pmuludq instructions)
  - [gcc -O3 -funroll-loops -m32](asm_listings/gcc-x86.lst#L300)
  - [gcc -O3 -funroll-loops -m32 -msse2 -DSSE2](asm_listings/gcc-x86-sse2.lst#L323)
  - [gcc -O3 -funroll-loops -m32 -mavx2 -DAVX2](asm_listings/gcc-x86-avx2.lst#L320)
  - [gcc -O3 -funroll-loops -m64 -DSSE2](asm_listings/gcc-x64.lst#L260)
  - [gcc -O3 -funroll-loops -m64 -mavx2 -DAVX2](asm_listings/gcc-x64-avx2.lst#L262)

## Benchmark on Haswell i7-4770
```
farsh32.exe:       6.457 GB/s =  6.014 GiB/s
farsh32sse2.exe:  30.436 GB/s = 28.346 GiB/s
farsh32avx2.exe:  58.783 GB/s = 54.746 GiB/s

farsh64.exe:      26.296 GB/s = 24.490 GiB/s
farsh64avx2.exe:  57.904 GB/s = 53.927 GiB/s
```
