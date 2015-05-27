# FARSH
Fast and reliable (but not secure) 32-bit hash. Longer hashes (of `32*n` bits, up to 1024 bits) can be calculated with `farsh_n()`. Main loop uses universal hashing formula from [UMAC](http://en.wikipedia.org/wiki/UMAC) with a precomputed key material of 1024 bytes. You can use the FARSH as keyed hash by calling `farsh_keyed()` with 1024-byte key or `farsh_keyed_n()` with key of `1024+(n-1)*4` bytes.

# Benchmark on Haswell i7-4770
```
C:\>gcc -std=c++11 -O3 -funroll-loops -s -static -m32 main.cpp & a
6.331 GB/s = 5.896 GiB/s

C:\>gcc -std=c++11 -O3 -funroll-loops -s -static -m32 -msse2 -DSSE2 main.cpp & a
31.207 GB/s = 29.063 GiB/s

C:\>gcc -std=c++11 -O3 -funroll-loops -s -static -m64 -msse2 -DSSE2 main.cpp & a
29.277 GB/s = 27.266 GiB/s

C:\>gcc -std=c++11 -O3 -funroll-loops -s -static -m64 -mavx2 -DAVX2 main.cpp & a
55.483 GB/s = 51.672 GiB/s
```

# Features
- [x] hashes up to 1024 bits long (`farsh_n`)
- [x] hashes with user-supplied key material (`farsh_keyed` and `farsh_keyed_n`)
- [x] SSE2/AVX2 manually-optimized main loop
- [ ] `farsh_init/farsh_update/farsh_result` streaming API
- [ ] further improve the hash quality by tuning `COMPRESS_ULONG()` and hashsum combining
