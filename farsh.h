#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint32_t & uint64_t */

/* Constants describing FARSH requirements to calling code */
#define FARSH_MAX_HASHES             32  /* number of 32-bit hashes supported by the built-in key */
#define FARSH_BASE_KEY_SIZE        1024  /* size of user-supplied key required to compute 32-bit hash with index 0 */
#define FARSH_EXTRA_KEY_SIZE         16  /* extra bytes required to compute 32-bit hash with every next index */
#define FARSH_BASE_KEY_ALIGNMENT     16  /* user-supplied key should be aligned to this size, otherwise SSE2 code may fail. For maximum speed, it's recommended to align key to 64 bytes. */


/* Hash the buffer. The 'seed' value in any function below may be used to "personalize" the hash function. Use seed==0 if you don't need that. */
uint32_t farsh (const void *data, size_t bytes, uint64_t seed);

/* Hash the buffer, computing 32*n bits long result, starting with hash number 'k' (k+n<=32), and write result to the buffer pointed by the hash parameter */
void farsh_n (const void *data, size_t bytes, int k, int n, uint64_t seed, void *hash);

/* Hash the buffer with the user-supplied 16byte-aligned 1024-byte key material */
uint32_t farsh_keyed (const void *data, size_t bytes, const void *key, uint64_t seed);

/* Hash the buffer with the user-supplied 16byte-aligned, 1008+n*16 bytes long key material, and write 32*n bits long result to the buffer pointed by the hash parameter */
void farsh_keyed_n (const void *data, size_t bytes, const void *key, int n, uint64_t seed, void *hash);
