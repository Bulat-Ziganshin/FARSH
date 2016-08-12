#include "Platform.h"
#include "Hashes.h"
#include "KeysetTest.h"
#include "SpeedTest.h"
#include "AvalancheTest.h"
#include "DifferentialTest.h"

#include <stdio.h>
#include <time.h>

//-----------------------------------------------------------------------------
// Configuration. TODO - move these to command-line flags

bool g_testAll = false;

bool g_testSanity      = false;
bool g_testSpeed       = false;
bool g_testDiff        = false;
bool g_testDiffDist    = false;
bool g_testAvalanche   = false;
bool g_testBIC         = false;
bool g_testCyclic      = false;
bool g_testTwoBytes    = false;
bool g_testSparse      = false;
bool g_testPermutation = false;
bool g_testWindow      = false;
bool g_testText        = false;
bool g_testZeroes      = false;
bool g_testSeed        = false;

//-----------------------------------------------------------------------------
// This is the list of all hashes that SMHasher can test.

struct HashInfo
{
  pfHash hash;
  int hashbits;
  uint32_t verification;
  const char * name;
  const char * desc;
};

HashInfo g_hashes[] =
{
  { DoNothingHash,        32, 0x00000000, "donothing32", "Do-Nothing function (only valid for measuring call overhead)" },
  { DoNothingHash,        64, 0x00000000, "donothing64", "Do-Nothing function (only valid for measuring call overhead)" },
  { DoNothingHash,       128, 0x00000000, "donothing128", "Do-Nothing function (only valid for measuring call overhead)" },

  { crc32,                32, 0x3719DB20, "crc32",       "CRC-32 soft" },

  { sha1_32,              32, 0x0C2E2F52, "sha1_32",     "SHA1, first 32 bits of result" },
  { sha1_32a,             32, 0x39D77F01, "sha1_32a",    "SHA1, second 32 bits of result" },
  { sha1_32b,             32, 0x8436EAEC, "sha1_32b",    "SHA1, third 32 bits of result" },
  { sha1_32c,             32, 0x3941812B, "sha1_32c",    "SHA1, thourth 32 bits of result" },
  { sha1_64,              64, 0x2DD8C16C, "sha1_64",     "SHA1, first 64 bits of result" },
  { sha1_64a,             64, 0x4E971A7E, "sha1_64a",    "SHA1, second 64 bits of result" },
  { sha1_128,            128, 0x3396C771, "sha1_128",    "SHA1, first 128 bits of result" },

  { farsh32_test,         32, 0xBCDE332C, "Farsh32",     "FARSH, 32-bit result" },
  { farsh64_test,         64, 0xDE2FDAEE, "Farsh64",     "FARSH, 64-bit result" },
  { farsh128_test,       128, 0x82B6CBEC, "Farsh128",    "FARSH, 128-bit result"},
  { farsh256_test,       256, 0xFEBEA0BC, "Farsh256",    "FARSH, 256-bit result"},

  { uhash32_test,         32, 0x852EACB6, "UHash32",     "UHASH, 32-bit result" },

  { vhash64_test,         64, 0x78C43DC5, "VHash64",     "VHASH, 64-bit result" },
  { vhash128_test,       128, 0xA5160150, "VHash128",    "VHASH, 128-bit result" },

  { poly1305_test,       128, 0x852EACB6, "poly1305",    "poly-1305, 128-bit result" },

  { XXH32_test,           32, 0xBA88B743, "XXH32",       "xxHash, 32-bit result" },
  { XXH64_test,           64, 0x024B7CF4, "XXH64",       "xxHash, 64-bit result" },

  { ModXXH32_test,        32, 0x6FE47417, "Mod32",       "xxHash with XXH64 finalization, 32-bit result" },
  { ModXXH32a_test,       32, 0xD68852E6, "Mod32a",      "xxHash with XXH64 finalization, upper 32 bits of result" },
  { ModXXH64_test,        64, 0xF5DA1DC5, "Mod64",       "xxHash with XXH64 finalization, 64-bit result" },

  { ZZH32_test,           32, 0x7683F96B, "ZZH32",       "zzHash, 32-bit result" },
  { ZZH32a_test,          32, 0x40BB02BF, "ZZH32a",      "zzHash, second 32 bits of result" },
  { ZZH32b_test,          32, 0x9091A211, "ZZH32b",      "zzHash, third 32 bits of result" },
  { ZZH32c_test,          32, 0x7E9077D2, "ZZH32c",      "zzHash, thourth 32 bits of result" },
  { ZZH64_test,           64, 0xC2B43E72, "ZZH64",       "zzHash, 64-bit result" },
  { ZZH64a_test,          64, 0x58C74F5A, "ZZH64a",      "zzHash, upper 64-bit result" },
  { ZZH128_test,         128, 0x66AF8C86, "ZZH128",      "zzHash, 128-bit result" },

  { SlowZZH32_test,       32, 0x38078AB0, "Slow32",      "Slow zzHash, 32-bit result" },
  { SlowZZH32a_test,      32, 0x44A367B4, "Slow32a",     "Slow zzHash, second 32 bits of result" },
  { SlowZZH32b_test,      32, 0x44A367B4, "Slow32b",     "Slow zzHash, third 32 bits of result" },
  { SlowZZH32c_test,      32, 0x44A367B4, "Slow32c",     "Slow zzHash, thourth 32 bits of result" },
  { SlowZZH64_test,       64, 0xFA028F55, "Slow64",      "Slow zzHash, 64-bit result" },
  { SlowZZH64a_test,      64, 0xFA028F55, "Slow64a",     "Slow zzHash, upper 64-bit result" },
  { SlowZZH128_test,     128, 0xFA028F55, "Slow128",     "Slow zzHash, 128-bit result" },

  { WideZZH32_test,       32, 0x7D354E34, "Wide32",      "x64-optimized zzHash, 32-bit result" },
  { WideZZH32a_test,      32, 0x8350E1EB, "Wide32a",     "x64-optimized zzHash, upper 32 bits of result" },
  { WideZZH64_test,       64, 0xAEC33A87, "Wide64",      "x64-optimized zzHash, 64-bit result" },

  { SlowWideZZH32_test,   32, 0xE62B820C, "SlowWide32",  "x64-optimized slow zzHash, 32-bit result" },
  { SlowWideZZH32a_test,  32, 0xE51F1FD5, "SlowWide32a", "x64-optimized slow zzHash, upper 32 bits of result" },
  { SlowWideZZH64_test,   64, 0x854F0860, "SlowWide64",  "x64-optimized slow zzHash, 64-bit result" },

  { SimdZZH32_test,       32, 0x7D354E34, "Simd32",      "SIMD-optimized zzHash, 32-bit result" },
  { SimdZZH32a_test,      32, 0x8350E1EB, "Simd32a",     "SIMD-optimized zzHash, upper 32 bits of result" },
  { SimdZZH64_test,       64, 0xAEC33A87, "Simd64",      "SIMD-optimized zzHash, 64-bit result" },

  { SpookyHash32_test,    32, 0xA48BE265, "Spooky32",    "SpookyHash V2, 32-bit result" },
  { SpookyHash32a_test,   32, 0x7B7AD2B9, "Spooky32a",   "SpookyHash V2, second 32 bits of result" },
  { SpookyHash32b_test,   32, 0x75691943, "Spooky32b",   "SpookyHash V2, third 32 bits of result" },
  { SpookyHash32c_test,   32, 0x5E47E7B3, "Spooky32c",   "SpookyHash V2, thourth 32 bits of result" },
  { SpookyHash64_test,    64, 0x972C4BDC, "Spooky64",    "SpookyHash V2, 64-bit result" },
  { SpookyHash128_test,  128, 0xB7C8659C, "Spooky128",   "SpookyHash V2, 128-bit result" },

  { MurmurHash3_x86_32,   32, 0xB0F57EE3, "Murmur3A",    "MurmurHash3 for x86, 32-bit" },
  { MurmurHash3_x86_128, 128, 0xB3ECE62A, "Murmur3C",    "MurmurHash3 for x86, 128-bit" },
  { MurmurHash3_x64_128, 128, 0x6384BA69, "Murmur3F",    "MurmurHash3 for x64, 128-bit" },

  { Murmur3c_32,          32, 0xB887C081, "Murmur32",    "MurmurHash3 for x86, first 32 bits of 128-bit result" },
  { Murmur3c_32a,         32, 0x92135E7E, "Murmur32a",   "MurmurHash3 for x86, second 32 bits of 128-bit result" },
  { Murmur3c_32b,         32, 0x4002DBAE, "Murmur32b",   "MurmurHash3 for x86, third 32 bits of 128-bit result" },
  { Murmur3c_32c,         32, 0x6BE72A6C, "Murmur32c",   "MurmurHash3 for x86, thourth 32 bits of 128-bit result" },
};

HashInfo * findHash ( const char * name )
{
  for(size_t i = 0; i < sizeof(g_hashes) / sizeof(HashInfo); i++)
  {
    if(_stricmp(name,g_hashes[i].name) == 0) return &g_hashes[i];
  }

  return NULL;
}

//-----------------------------------------------------------------------------
// Self-test on startup - verify that all installed hashes work correctly.

void SelfTest ( void )
{
  bool pass = true;

  for(size_t i = 0; i < sizeof(g_hashes) / sizeof(HashInfo); i++)
  {
    HashInfo * info = & g_hashes[i];

/////////////////////////    pass &= VerificationTest(info->hash,info->hashbits,info->verification,false);
  }

  if(!pass)
  {
    printf("Self-test FAILED!\n");

    for(size_t i = 0; i < sizeof(g_hashes) / sizeof(HashInfo); i++)
    {
      HashInfo * info = & g_hashes[i];

      printf("%16s - ",info->name);
      pass &= VerificationTest(info->hash,info->hashbits,info->verification,true);
    }

    exit(1);
  }
}

//----------------------------------------------------------------------------

template < typename hashtype >
void test ( hashfunc<hashtype> hash, HashInfo * info )
{
  const int hashbits = sizeof(hashtype) * 8;

  printf("-------------------------------------------------------------------------------\n");
  printf("--- Testing %s (%s)\n\n",info->name,info->desc);

  //-----------------------------------------------------------------------------
  // Sanity tests

  if(g_testSanity || g_testAll)
  {
    printf("[[[ Sanity Tests ]]]\n\n");

    VerificationTest(hash,hashbits,info->verification,true);
    SanityTest(hash,hashbits);
    AppendedZeroesTest(hash,hashbits);
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Speed tests

  if(g_testSpeed || g_testAll)
  {
    printf("[[[ Speed Tests ]]]\n\n");

    SetAffinity((1 << 2));   // Run speed tests on 3rd CPU to ensure that RDTSC always the the same core

    BulkSpeedTest(info->hash,info->hashbits,info->verification);
    printf("\n");

    const int max_keysize = 66;
    TinySpeedTest(info->hash,info->hashbits,max_keysize,info->verification,true);

    ResetAffinity();   // Use all cores for the rest of code

    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Avalanche tests

  if(g_testAvalanche || g_testAll)
  {
    printf("[[[ Avalanche Tests ]]]\n\n");

    bool result = true;

    result &= AvalancheTest< Blob< 32>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob< 40>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob< 48>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob< 56>, hashtype > (hash,300000);

    result &= AvalancheTest< Blob< 64>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob< 72>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob< 80>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob< 88>, hashtype > (hash,300000);

    result &= AvalancheTest< Blob< 96>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob<104>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob<112>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob<120>, hashtype > (hash,300000);

    result &= AvalancheTest< Blob<128>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob<136>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob<144>, hashtype > (hash,300000);
    result &= AvalancheTest< Blob<152>, hashtype > (hash,300000);

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Differential tests

  if(g_testDiff || g_testAll)
  {
    printf("[[[ Differential Tests ]]]\n\n");

    bool result = true;
    bool dumpCollisions = false;

    result &= DiffTest< Blob<64>,  hashtype >(hash,5,1000,dumpCollisions);
    result &= DiffTest< Blob<128>, hashtype >(hash,4,1000,dumpCollisions);
    result &= DiffTest< Blob<256>, hashtype >(hash,3,1000,dumpCollisions);

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Differential-distribution tests

  if(g_testDiffDist /*|| g_testAll*/)
  {
    printf("[[[ Differential Distribution Tests ]]]\n\n");

    bool result = true;

    result &= DiffDistTest2<uint64_t,hashtype>(hash);

    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Bit Independence Criteria. Interesting, but doesn't tell us much about
  // collision or distribution.

  if(g_testBIC)
  {
    printf("[[[ Bit Independence Criteria ]]]\n\n");

    bool result = true;

    //result &= BicTest<uint64_t,hashtype>(hash,2000000);
    BicTest3<Blob<88>,hashtype>(hash,2000000);

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Keyset 'Cyclic' - keys of the form "abcdabcdabcd..."

  if(g_testCyclic || g_testAll)
  {
    printf("[[[ Keyset 'Cyclic' Tests ]]]\n\n");

    bool result = true;
    bool drawDiagram = false;

    result &= CyclicKeyTest<hashtype>(hash,sizeof(hashtype)+0,8,10000000,drawDiagram);
    result &= CyclicKeyTest<hashtype>(hash,sizeof(hashtype)+1,8,10000000,drawDiagram);
    result &= CyclicKeyTest<hashtype>(hash,sizeof(hashtype)+2,8,10000000,drawDiagram);
    result &= CyclicKeyTest<hashtype>(hash,sizeof(hashtype)+3,8,10000000,drawDiagram);
    result &= CyclicKeyTest<hashtype>(hash,sizeof(hashtype)+4,8,10000000,drawDiagram);

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Keyset 'TwoBytes' - all keys up to N bytes containing two non-zero bytes

  // This generates some huge keysets, 128-bit tests will take ~1.3 gigs of RAM.

  if(g_testTwoBytes || g_testAll)
  {
    printf("[[[ Keyset 'TwoBytes' Tests ]]]\n\n");

    bool result = true;
    bool drawDiagram = false;

    for(int i = 4; i <= 20; i += 4)
    {
      result &= TwoBytesTest2<hashtype>(hash,i,drawDiagram);
    }

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Keyset 'Sparse' - keys with all bits 0 except a few

  if(g_testSparse || g_testAll)
  {
    printf("[[[ Keyset 'Sparse' Tests ]]]\n\n");

    bool result = true;
    bool drawDiagram = false;

    result &= SparseKeyTest<  32,hashtype>(hash,6,true,true,true,drawDiagram);
    result &= SparseKeyTest<  40,hashtype>(hash,6,true,true,true,drawDiagram);
    result &= SparseKeyTest<  48,hashtype>(hash,5,true,true,true,drawDiagram);
    result &= SparseKeyTest<  56,hashtype>(hash,5,true,true,true,drawDiagram);
    result &= SparseKeyTest<  64,hashtype>(hash,5,true,true,true,drawDiagram);
    result &= SparseKeyTest<  96,hashtype>(hash,4,true,true,true,drawDiagram);
    result &= SparseKeyTest< 256,hashtype>(hash,3,true,true,true,drawDiagram);
    result &= SparseKeyTest<2048,hashtype>(hash,2,true,true,true,drawDiagram);

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Keyset 'Permutation' - all possible combinations of a set of blocks

  if(g_testPermutation || g_testAll)
  {
    {
      // This one breaks lookup3, surprisingly

      printf("[[[ Keyset 'Combination Lowbits' Tests ]]]\n\n");

      bool result = true;
      bool drawDiagram = false;

      uint32_t blocks[] =
      {
        0x00000000,

        0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007,
      };

      result &= CombinationKeyTest<hashtype>(hash,8,blocks,sizeof(blocks) / sizeof(uint32_t),true,true,drawDiagram);

      if(!result) printf("*********FAIL*********\n");
      printf("\n");
    }

    {
      printf("[[[ Keyset 'Combination Highbits' Tests ]]]\n\n");

      bool result = true;
      bool drawDiagram = false;

      uint32_t blocks[] =
      {
        0x00000000,

        0x20000000, 0x40000000, 0x60000000, 0x80000000, 0xA0000000, 0xC0000000, 0xE0000000
      };

      result &= CombinationKeyTest<hashtype>(hash,8,blocks,sizeof(blocks) / sizeof(uint32_t),true,true,drawDiagram);

      if(!result) printf("*********FAIL*********\n");
      printf("\n");
    }

    {
      printf("[[[ Keyset 'Combination 0x8000000' Tests ]]]\n\n");

      bool result = true;
      bool drawDiagram = false;

      uint32_t blocks[] =
      {
        0x00000000,

        0x80000000,
      };

      result &= CombinationKeyTest<hashtype>(hash,20,blocks,sizeof(blocks) / sizeof(uint32_t),true,true,drawDiagram);

      if(!result) printf("*********FAIL*********\n");
      printf("\n");
    }

    {
      printf("[[[ Keyset 'Combination 0x0000001' Tests ]]]\n\n");

      bool result = true;
      bool drawDiagram = false;

      uint32_t blocks[] =
      {
        0x00000000,

        0x00000001,
      };

      result &= CombinationKeyTest<hashtype>(hash,20,blocks,sizeof(blocks) / sizeof(uint32_t),true,true,drawDiagram);

      if(!result) printf("*********FAIL*********\n");
      printf("\n");
    }

    {
      printf("[[[ Keyset 'Combination Hi-Lo' Tests ]]]\n\n");

      bool result = true;
      bool drawDiagram = false;

      uint32_t blocks[] =
      {
        0x00000000,

        0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007,

        0x80000000, 0x40000000, 0xC0000000, 0x20000000, 0xA0000000, 0x60000000, 0xE0000000
      };

      result &= CombinationKeyTest<hashtype>(hash,6,blocks,sizeof(blocks) / sizeof(uint32_t),true,true,drawDiagram);

      if(!result) printf("*********FAIL*********\n");
      printf("\n");
    }
  }

  //-----------------------------------------------------------------------------
  // Keyset 'Window'

  // Skip distribution test for these - they're too easy to distribute well,
  // and it generates a _lot_ of testing

  if(g_testWindow || g_testAll)
  {
    printf("[[[ Keyset 'Window' Tests ]]]\n\n");

    bool result = true;
    bool testCollision = true;
    bool testDistribution = false;
    bool drawDiagram = false;

    result &= WindowedKeyTest< Blob<hashbits*2>, hashtype > ( hash, 20, testCollision, testDistribution, drawDiagram );

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Keyset 'Text'

  if(g_testText || g_testAll)
  {
    printf("[[[ Keyset 'Text' Tests ]]]\n\n");

    bool result = true;
    bool drawDiagram = false;

    const char * alnum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    result &= TextKeyTest( hash, "Foo",    alnum,4, "Bar",    drawDiagram );
    result &= TextKeyTest( hash, "FooBar", alnum,4, "",       drawDiagram );
    result &= TextKeyTest( hash, "",       alnum,4, "FooBar", drawDiagram );

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Keyset 'Zeroes'

  if(g_testZeroes || g_testAll)
  {
    printf("[[[ Keyset 'Zeroes' Tests ]]]\n\n");

    bool result = true;
    bool drawDiagram = false;

    result &= ZeroKeyTest<hashtype>( hash, drawDiagram );

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }

  //-----------------------------------------------------------------------------
  // Keyset 'Seed'

  if(g_testSeed || g_testAll)
  {
    printf("[[[ Keyset 'Seed' Tests ]]]\n\n");

    bool result = true;
    bool drawDiagram = false;

    result &= SeedTest<hashtype>( hash, 1000000, drawDiagram );

    if(!result) printf("*********FAIL*********\n");
    printf("\n");
  }
}

//-----------------------------------------------------------------------------

uint32_t g_inputVCode = 1;
uint32_t g_outputVCode = 1;
uint32_t g_resultVCode = 1;

HashInfo * g_hashUnderTest = NULL;

void VerifyHash ( const void * key, int len, uint32_t seed, void * out )
{
  g_inputVCode = MurmurOAAT(key,len,g_inputVCode);
  g_inputVCode = MurmurOAAT(&seed,sizeof(uint32_t),g_inputVCode);

  g_hashUnderTest->hash(key,len,seed,out);

  g_outputVCode = MurmurOAAT(out,g_hashUnderTest->hashbits/8,g_outputVCode);
}

//-----------------------------------------------------------------------------

void testHash ( const char * name )
{
  HashInfo * pInfo = findHash(name);

  if(pInfo == NULL)
  {
    printf("Invalid hash '%s' specified\n",name);
    return;
  }
  else
  {
    g_hashUnderTest = pInfo;

    if(pInfo->hashbits == 32)
    {
      test<uint32_t>( VerifyHash, pInfo );
    }
    else if(pInfo->hashbits == 64)
    {
      test<uint64_t>( pInfo->hash, pInfo );
    }
    else if(pInfo->hashbits == 128)
    {
      test<uint128_t>( pInfo->hash, pInfo );
    }
    else if(pInfo->hashbits == 256)
    {
      test<uint256_t>( pInfo->hash, pInfo );
    }
    else
    {
      printf("Invalid hash bit width %d for hash '%s'",pInfo->hashbits,pInfo->name);
    }
  }
}
//-----------------------------------------------------------------------------

int main ( int argc, char ** argv )
{
  const char * hashToTest = "farsh32";

  if(argc < 2)
  {
    printf("(No test hash given on command line, testing farsh32)\n");
  }
  else
  {
    hashToTest = argv[1];
  }

  SelfTest();

  int timeBegin = clock();

  g_testAll = true;

  //g_testSanity = true;
  //g_testSpeed = true;
  //g_testAvalanche = true;
  //g_testBIC = true;
  //g_testCyclic = true;
  //g_testTwoBytes = true;
  //g_testDiff = true;
  //g_testDiffDist = true;
  //g_testSparse = true;
  //g_testPermutation = true;
  //g_testWindow = true;
  //g_testZeroes = true;

  testHash(hashToTest);

  //----------

  int timeEnd = clock();

  printf("\n");
  printf("Input vcode 0x%08x, Output vcode 0x%08x, Result vcode 0x%08x\n",g_inputVCode,g_outputVCode,g_resultVCode);
  printf("Verification value is 0x%08x - Testing took %f seconds\n",g_verify,double(timeEnd-timeBegin)/double(CLOCKS_PER_SEC));
  printf("-------------------------------------------------------------------------------\n");
  return 0;
}
