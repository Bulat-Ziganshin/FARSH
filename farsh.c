#include <stdlib.h>
#include <string.h>

#define UINT  /*uint32_t*/ unsigned
#define ULONG /*uint64_t*/ unsigned long long

#define STRIPE 1024
#define STRIPE_ELEMENTS (STRIPE/sizeof(UINT))
static const UINT farsh_key[STRIPE_ELEMENTS] = {0};

// Hash up to STRIPE bytes, consisting of whole UINT pairs, including optional UINT pair in the extra[]
UINT farsh_pairs (UINT *data, size_t elements, UINT* extra)
{
    ULONG sum = 0;  int i;
    for (i=0; i < elements; i+=2)
        sum += (data[i] + farsh_key[i]) * (ULONG)(data[i+1] + farsh_key[i+1]);
    if (extra)
        sum += (extra[0] + farsh_key[i]) * (ULONG)(extra[1] + farsh_key[i+1]);
    return (UINT)(sum >> 32);
}

// Hash up to STRIPE bytes, with special optimization for exactly STRIPE input bytes
// and careful handling of partial UINT pair at the end of buffer
UINT farsh_block (void *data_ptr, size_t bytes)
{
    UINT *data = (UINT*) data_ptr;
    if (bytes == STRIPE)  return farsh_pairs (data, STRIPE_ELEMENTS, NULL);
    size_t elements = (bytes/sizeof(UINT)) & (~1);
    UINT extra_data[2] = {0};  size_t extra_bytes = bytes - elements*sizeof(UINT);
    memcpy (extra_data, data+elements, extra_bytes);
    return farsh_pairs (data, elements, extra_bytes?extra_data:NULL);
}

// Hash buffer of arbitrary size
UINT farsh (void *data_ptr, size_t bytes)
{
    ULONG sum = 0;  int i = 0;
    char *data = (char*) data_ptr,  *end = data+bytes;
    while (data < end)
    {
        size_t minbytes = (bytes<STRIPE? bytes : STRIPE);
        UINT h = farsh_block (data, minbytes);
        data += minbytes;
        sum += h*(ULONG)farsh_key[i];
        if (++i == STRIPE_ELEMENTS)
            i = 0;//to do
    }
    return (UINT)(sum >> 32);
}

#undef UINT
#undef ULONG
