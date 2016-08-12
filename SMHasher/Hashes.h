#pragma once

#include "Types.h"
#include "MurmurHash3.h"

//----------
// These are _not_ hash functions (even though people tend to use crc32 as one...)

void DoNothingHash ( const void * key, int len, uint32_t seed, void * out );

//----------
// General purpose hashes

uint32_t MurmurOAAT ( const void * key, int len, uint32_t seed );

void farsh32_test  ( const void * key, int len, unsigned seed, void * out );
void farsh64_test  ( const void * key, int len, unsigned seed, void * out );
void farsh128_test ( const void * key, int len, unsigned seed, void * out );
void farsh256_test ( const void * key, int len, unsigned seed, void * out );

void uhash32_test  ( const void * key, int len, unsigned seed, void * out );

void vhash64_test  ( const void * key, int len, unsigned seed, void * out );
void vhash128_test ( const void * key, int len, unsigned seed, void * out );

void poly1305_test ( const void * key, int len, unsigned seed, void * out );

//-----------------------------------------------------------------------------
// SpookyHashV2 and its 32-bit parts

void SpookyHash32_test ( const void * key, int len, unsigned seed, void * out );
void SpookyHash32a_test( const void * key, int len, unsigned seed, void * out );
void SpookyHash32b_test( const void * key, int len, unsigned seed, void * out );
void SpookyHash32c_test( const void * key, int len, unsigned seed, void * out );
void SpookyHash64_test ( const void * key, int len, unsigned seed, void * out );
void SpookyHash128_test( const void * key, int len, unsigned seed, void * out );

//-----------------------------------------------------------------------------
// 32-bit parts of MurmurHash3_x86_128

void Murmur3c_32 ( const void * key, const int len, uint32_t seed, void * out );
void Murmur3c_32a( const void * key, const int len, uint32_t seed, void * out );
void Murmur3c_32b( const void * key, const int len, uint32_t seed, void * out );
void Murmur3c_32c( const void * key, const int len, uint32_t seed, void * out );

//-----------------------------------------------------------------------------
// xxHash

void XXH32_test ( const void * key, int len, unsigned seed, void * out );
void XXH64_test ( const void * key, int len, unsigned seed, void * out );

//-----------------------------------------------------------------------------
// XXH32 with XXH64 backend

void ModXXH32_test   ( const void * key, int len, unsigned seed, void * out );
void ModXXH32a_test  ( const void * key, int len, unsigned seed, void * out );
void ModXXH64_test   ( const void * key, int len, unsigned seed, void * out );

//-----------------------------------------------------------------------------
// ZZH & SlowZZH: my experimental x86-optimized hashes

void ZZH32_test      ( const void * key, int len, unsigned seed, void * out );
void ZZH32a_test     ( const void * key, int len, unsigned seed, void * out );
void ZZH32b_test     ( const void * key, int len, unsigned seed, void * out );
void ZZH32c_test     ( const void * key, int len, unsigned seed, void * out );
void ZZH64_test      ( const void * key, int len, unsigned seed, void * out );
void ZZH64a_test     ( const void * key, int len, unsigned seed, void * out );
void ZZH128_test     ( const void * key, int len, unsigned seed, void * out );

void SlowZZH32_test  ( const void * key, int len, unsigned seed, void * out );
void SlowZZH32a_test ( const void * key, int len, unsigned seed, void * out );
void SlowZZH32b_test ( const void * key, int len, unsigned seed, void * out );
void SlowZZH32c_test ( const void * key, int len, unsigned seed, void * out );
void SlowZZH64_test  ( const void * key, int len, unsigned seed, void * out );
void SlowZZH64a_test ( const void * key, int len, unsigned seed, void * out );
void SlowZZH128_test ( const void * key, int len, unsigned seed, void * out );

//-----------------------------------------------------------------------------
// WideZZH & SlowWideZZH: my experimental x64-optimized hashes

void WideZZH32_test  ( const void * key, int len, unsigned seed, void * out );
void WideZZH32a_test ( const void * key, int len, unsigned seed, void * out );
void WideZZH64_test  ( const void * key, int len, unsigned seed, void * out );

void SlowWideZZH32_test  ( const void * key, int len, unsigned seed, void * out );
void SlowWideZZH32a_test ( const void * key, int len, unsigned seed, void * out );
void SlowWideZZH64_test  ( const void * key, int len, unsigned seed, void * out );

//-----------------------------------------------------------------------------
// SimdZZH: my experimental SIMD-optimized hashe

void SimdZZH32_test  ( const void * key, int len, unsigned seed, void * out );
void SimdZZH32a_test ( const void * key, int len, unsigned seed, void * out );
void SimdZZH64_test  ( const void * key, int len, unsigned seed, void * out );
