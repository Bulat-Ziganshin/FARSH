#pragma once

#include "Types.h"

void BulkSpeedTest ( pfHash hash, int hashsize, uint32_t seed );
void TinySpeedTest ( pfHash hash, int hashsize, int max_keysize, uint32_t seed, bool verbose );

//-----------------------------------------------------------------------------
