#pragma once

#include "Types.h"

//----------
// These are _not_ hash functions (even though people tend to use crc32 as one...)

void DoNothingHash         ( const void * key, int len, uint32_t seed, void * out );

//----------
// General purpose hashes

uint32_t MurmurOAAT ( const void * key, int len, uint32_t seed );

//-----------------------------------------------------------------------------
// Test harnesses for FARSH

void farsh32_test  ( const void * key, int len, unsigned seed, void * out );
void farsh64_test  ( const void * key, int len, unsigned seed, void * out );
void farsh128_test ( const void * key, int len, unsigned seed, void * out );
void farsh256_test ( const void * key, int len, unsigned seed, void * out );

//-----------------------------------------------------------------------------
// Test harnesses for UHASH

void uhash32_test ( const void * key, int len, unsigned seed, void * out );

//-----------------------------------------------------------------------------
// Test harnesses for VHASH

void vhash64_test ( const void * key, int len, unsigned seed, void * out );
