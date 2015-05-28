#pragma once

#include "Types.h"

//----------
// These are _not_ hash functions (even though people tend to use crc32 as one...)

void DoNothingHash         ( const void * key, int len, uint32_t seed, void * out );

//----------
// General purpose hashes

uint32_t MurmurOAAT ( const void * key, int len, uint32_t seed );
