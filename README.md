# FARSH
Fast and reliable (but not cryptographic) scalable hash

Features:
- [x] hashes up to 1024 bits long (farsh_n)
- [x] hashes with user-supplied key material (farsh_key and farsh_key_n)
- [x] SSE2/AVX2 manually-optimized main loop
- [ ] farsh_init/farsh_update/farsh_result streaming API
- [ ] better high-level mixing
- [ ] ? improve COMPRESS_ULONG (crc32(sum)?)
