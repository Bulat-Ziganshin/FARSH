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
- [ ] try PSLLQ instead of PSHUFD in SSE2 code to [improve speed on older CPUs](http://encode.ru/threads/2213-FARSH-hashing-30-GB-s!?p=43983&viewfull=1#post43983)
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
[Benchmark](benchmark) measures overall hash speed as well as internal loop speed.
The internal loop speed is a hard limit for the speed of any future FARSH version,
while the overall speed includes time required for pretty slow high-level hashing.
Future versions should replace it with faster algorithm still satisfying the [SMHasher] requirements,
making overall hash speed within 10% of the internal loop speed.

Executables were [compiled](benchmark/compile.cmd) with GCC 4.9.2.
Aligned versions make sure that data being hashed are 64-byte aligned,
unaligned versions make sure that data are unaligned.
This makes big difference on Core2 and older Intel CPUs.

[Intel Haswell i7-4770 3.9 GHz (AVX2)](http://ark.intel.com/products/75122/Intel-Core-i7-4770-Processor-8M-Cache-up-to-3_90-GHz),
other IvyBridge to Skylake CPUs has pretty close performance/GHz:

Executable                |  FARSH 0.2 speed             |  Internal loop speed
--------------------------|-----------------------------:|----------------------------:
aligned-farsh-x64-avx2    |  54.536 GB/s = 50.790 GiB/s  |  65.645 GB/s = 61.137 GiB/s
aligned-farsh-x64         |  31.162 GB/s = 29.022 GiB/s  |  35.722 GB/s = 33.269 GiB/s
aligned-farsh-x86-avx2    |  40.279 GB/s = 37.513 GiB/s  |  61.682 GB/s = 57.446 GiB/s
aligned-farsh-x86-sse2    |  25.221 GB/s = 23.489 GiB/s  |  33.584 GB/s = 31.277 GiB/s
aligned-farsh-x86         |   6.255 GB/s =  5.825 GiB/s  |   6.336 GB/s =  5.901 GiB/s
||
farsh-x64-avx2            |  46.024 GB/s = 42.863 GiB/s  |  64.967 GB/s = 60.505 GiB/s
farsh-x64                 |  30.335 GB/s = 28.252 GiB/s  |  34.891 GB/s = 32.495 GiB/s
farsh-x86-avx2            |  35.273 GB/s = 32.851 GiB/s  |  57.252 GB/s = 53.320 GiB/s
farsh-x86-sse2            |  24.502 GB/s = 22.820 GiB/s  |  33.325 GB/s = 31.037 GiB/s
farsh-x86                 |   6.283 GB/s =  5.852 GiB/s  |   6.763 GB/s =  6.299 GiB/s


[Intel Pentium M processor 1.5 GHz (SSE2)](http://ark.intel.com/products/27576/Intel-Pentium-M-Processor-1_50-GHz-1M-Cache-400-MHz-FSB):

Executable                |  FARSH 0.2 speed             |  Internal loop speed
--------------------------|-----------------------------:|----------------------------:
aligned-farsh-x86-sse2    |   2.625 GB/s = 2.444 GiB/s   |   2.791 GB/s = 2.5 GiB/s
aligned-farsh-x86         |   1.664 GB/s = 1.550 GiB/s   |   1.946 GB/s = 1.8 GiB/s
||
farsh-x86-sse2            |   2.025 GB/s = 1.886 GiB/s   |   2.302 GB/s = 2.1 GiB/s
farsh-x86                 |   1.471 GB/s = 1.370 GiB/s   |   1.715 GB/s = 1.5 GiB/s


K10: [AMD Athlon II X2 220 Processor 2.8 GHz (SSE3)](http://www.cpu-world.com/CPUs/K10/AMD-Athlon%20II%20X2%20220%20-%20ADX220OCK22GM.html):

Executable                |  FARSH 0.2 speed             |  Internal loop speed
--------------------------|-----------------------------:|----------------------------:
aligned-farsh-x64         |  11.300 GB/s = 10.524 GiB/s  |  14.446 GB/s = 13.454 GiB/s
aligned-farsh-x86-sse2    |  10.899 GB/s = 10.151 GiB/s  |  13.280 GB/s = 12.368 GiB/s
aligned-farsh-x86         |   3.805 GB/s =  3.544 GiB/s  |   5.089 GB/s =  4.740 GiB/s
||
farsh-x64                 |  12.823 GB/s = 11.943 GiB/s  |  14.187 GB/s = 13.212 GiB/s
farsh-x86-sse2            |  10.933 GB/s = 10.182 GiB/s  |  12.389 GB/s = 11.538 GiB/s
farsh-x86                 |   3.786 GB/s =  3.526 GiB/s  |   5.825 GB/s =  5.425 GiB/s


Piledriver: [AMD A8-5500 APU 3.7 GHz (AVX)](http://www.cpu-world.com/CPUs/Bulldozer/AMD-A8-Series%20A8-5500.html):

Executable                |  FARSH 0.2 speed             |  Internal loop speed
--------------------------|-----------------------------:|----------------------------:
aligned-farsh-x64         |  17.130 GB/s = 15.953 GiB/s  |  21.394 GB/s = 19.924 GiB/s
aligned-farsh-x86-sse2    |  13.790 GB/s = 12.843 GiB/s  |  20.830 GB/s = 19.400 GiB/s
aligned-farsh-x86         |   3.872 GB/s =  3.606 GiB/s  |   5.457 GB/s =  5.082 GiB/s
||
farsh-x64                 |  15.313 GB/s = 14.262 GiB/s  |  19.659 GB/s = 18.309 GiB/s
farsh-x86-sse2            |  13.812 GB/s = 12.863 GiB/s  |  18.977 GB/s = 17.674 GiB/s
farsh-x86                 |   3.959 GB/s =  3.687 GiB/s  |   5.056 GB/s =  4.709 GiB/s

More results and benchmarking executables are available in those [forum posts](http://encode.ru/threads/2213-FARSH-hashing-30-GB-s!?p=48907&viewfull=1#post48907).



# Competition
Fast non-cryptographic hashes:
- {MumHash](https://github.com/vnmakarov/mum-hash) (2016)
- [HighwayHash](https://github.com/google/highwayhash) (2016)
- [CLHash](http://lemire.me/blog/2015/10/26/crazily-fast-hashing-with-carry-less-multiplications),
even [faster with Broadwell](http://lemire.me/blog/2015/12/24/your-software-should-follow-your-hardware-the-clhash-example) (2015)
- [MetroHash](https://github.com/jandrewrogers/MetroHash) (2015)
- Go language [32-bit](https://github.com/golang/go/blob/master/src/runtime/hash32.go) and [64-bit](https://github.com/golang/go/blob/master/src/runtime/hash64.go) hashes (2014)
- [xxHash][xxHash] (2012) and [xxHash64][xxHash64] (2014)
- [SpookyHash](http://burtleburtle.net/bob/hash/spooky.html): a 128-bit noncryptographic hash (2012)
- The [CityHash](https://github.com/google/cityhash) family of hash functions (2011)
- [MurmurHash3](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp) (2011)
- [Hasshe2](http://cessu.blogspot.ru/2008/11/hashing-with-sse2-revisited-or-my-hash.html) by Cessu (2008)

Further reading:
- [More info](https://github.com/aappleby/smhasher/wiki/SMHasher) about the [SMHasher] testsuite
- [A lot of hashes](https://github.com/rurban/smhasher) tested by SMHasher (see doc subdir)
- Interesting historical [overview](http://blog.reverberate.org/2012/01/state-of-hash-functions-2012.html)
- [SuperFastHash](http://www.azillionmonkeys.com/qed/hash.html)
- Bob Jenkins [1997 Dr Dobbs article](http://www.burtleburtle.net/bob/hash/doobs.html) and its [extended version](http://burtleburtle.net/bob/hash/evahash.html)

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
