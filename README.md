# FARSH
Fast and reliable (but not cryptographic) scalable hash

Features:
- [x] hashes up to 1024 bits long (farsh_n)
- [x] hashes with user-supplied key material (farsh_key and farsh_key_n)
- [ ] manually SSE2/AVX2-optimized implementation
- [ ] better high-level mixing
- [ ] ? replace 'sum>>32' with something else (sum>>32+sum or crc32(sum))
