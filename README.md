# FARSH
Fast and reliable (but not cryptographic) scalable hash

# Benchmarks
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
- [x] hashes up to 1024 bits long (farsh_n)
- [x] hashes with user-supplied key material (farsh_key and farsh_key_n)
- [x] SSE2/AVX2 manually-optimized main loop
- [ ] farsh_init/farsh_update/farsh_result streaming API
- [ ] better high-level mixing
- [ ] ? improve COMPRESS_ULONG (may be crc32(sum)?)
