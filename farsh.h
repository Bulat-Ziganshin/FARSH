/* Hash the buffer with the user-supplied 16byte-aligned 1024-byte key material */
unsigned farsh_keyed (const void *data, size_t bytes, const void *key);

/* Hash the buffer with the user-supplied 16byte-aligned, 1008+n*16 bytes long key material and return hash of 32*n bits long */
void farsh_keyed_n (const void *data, size_t bytes, const void *key, int n, void *hash);

/* Hash the buffer and return hash 32*n bits long (n<=32), starting with hash number 'k' */
void farsh_n (const void *data, size_t bytes, int k, int n, void *hash);

/* Hash the buffer */
unsigned farsh (const void *data, size_t bytes);
