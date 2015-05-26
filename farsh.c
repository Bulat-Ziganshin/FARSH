#include <stdlib.h>
#include <memory.h>

#define UINT  /*uint32_t*/ unsigned
#define ULONG /*uint64_t*/ unsigned long long

#define STRIPE          1024
#define STRIPE_ELEMENTS (STRIPE/sizeof(UINT))
static const UINT farsh_key[STRIPE_ELEMENTS+1024/32-1] = {  /* STRIPE bytes of key material plus extra keys for hashes up to 1024 bits long */
    0xb8fe6c39,0x23a44bbe,0x7c01812c,0xf721ad1c,0xded46de9,0x839097db,0x7240a4a4,0xb7b3671f,0xcb79e64e,0xccc0e578,0x825ad07d,0xccff7221,0xb8084674,0xf743248e,0xe03590e6,0x813a264c,0x3c2852bb,0x91c300cb,0x88d0658b,0x1b532ea3,0x71644897,0xa20df94e,0x3819ef46,0xa9deacd8,0xa8fa763f,0xe39c343f,0xf9dcbbc7,0xc70b4f1d,0x8a51e04b,0xcdb45931,0xc89f7ec9,0xd9787364,0x4f6a0752,0xa79b079c,0x8fc49499,0x8ec9b7a9,0x33c92249,0x4eb6404f,0xfb2afb4e,0xa4814255,0x2f0e1b98,0xace93b24,0x188850cd,0x6c5c74a7,0x66fa4404,0xeac5ac83,0x34d3ebc3,0xc581a0ff,0xfa1363eb,0x170ddd51,0xb7f0da49,0xd3165526,0x29d4689e,0x2b16be58,0x7d47a1fc,0x8ff8b8d1,0x7ad031ce,0x45cb3a8f,0x95160428,0xafd7fbca,0xbb4b407e,0x995274a4,0xeb9a2d93,0x3be78908,0xed475f6c,0x919cd8f2,0xd3861e5a,0x6e31390c,0xfe6a3a49,0xdcad0914,0x06508beb,0xa88399f3,0xb058112f,0xe8b0fa79,0x29b4da06,0xedc253fb,0xc3e96dad,0x6e372b83,0x4f78b153,0xfffa6e86,0x21beeeec,0x01caea02,0x1267e50d,0x11e6092f,0xe819d298,0x832f80dd,0x0c4e2477,0xbc7886eb,0x01506637,0x8ba89668,0x6d11e7a0,0xfc12fd15,0x86a54c19,0x593ce3dd,0xd2b13fe5,0x8e772b53,0xae4a60cc,0x647a3b1b,0x547786e0,0x3ec4378e,0x8d7acf89,0xca36f947,0x0e89d5ef,0xaada6a3c,0x6da4a109,0x9ac6e11c,0x686691ef,0xa357bd2b,0xd16f1b9a,0x38c70303,0x7d4622b3,0x2968fa8f,0x8ca5bcb9,0xfcd61005,0x228b5e96,0x2c9dcc19,0x57cf243c,0x3c53f9c1,0x0cc7952c,0x686de4f0,0x93a747b5,0x4e87a510,0x975e91ae,0x4c10b98e,0x8a7f068c,0x346b19ab,0x353ca625,0xf20a50e0,0xce9921f6,0xdf66e014,0x0a11ef4b,0x8bc84ddf,0x84d25d22,0xc823936d,0x94741ec3,0x88278a60,0xb8649331,0x7a707a10,0x7292cad6,0xa7c644c2,0xbd156bfa,0x646c9578,0xb7f4dfd5,0x9f8277a7,0x7013924e,0xad674cc3,0x2cae9d05,0x912a9a22,0xf67c53fa,0x8d7e22a9,0x59ae372b,0x850199f3,0x63a2102c,0xd6ff1261,0x56738ee1,0xaa95145b,0xfdd12832,0x5b684deb,0x0784de94,0xaa62390e,0xbb7ccf19,0x0fefd572,0x565b41ca,0x2206d202,0x2d608479,0x4c0fcd3d,0xd36d3be3,0x155a9a65,0x10f9e732,0xac9b0f1e,0x1f72a03b,0xea9440ae,0x5b674b4f,0x31a827d1,0xecca954f,0x3d2cd61e,0x768d3da4,0x93745ac1,0x1d5d58cb,0x4b86f3b6,0x2aba923a,0x0e65814c,0x8ae063d9,0xcd6969b0,0x36641585,0x742af59d,0x613a1316,0x338ea471,0x47861af3,0x30479dc3,0x1270a481,0x08771069,0xe3c4f0d2,0x0229874c,0x5a8a3bc1,0xe30d9733,0xd05be5a2,0xe2af31ba,0x222049f9,0x9f923b6a,0x033f64ec,0xe528b62b,0x8201efbd,0x2107d877,0xd8312ef1,0xa5679f99,0x1730b51b,0x752616d2,0x05305909,0x0dca440b,0x2093cdd9,0x6409ab50,0xba5c8ecc,0x8d4708ea,0x429f0917,0xb762fab0,0x5161ea75,0x45eba0eb,0xb6f34b41,0x52047123,0xe4181523,0x8d74e90a,0x54fa401c,0xddda0cc7,0x63df182a,0xc6403ef6,0x348ec6e8,0xb9ff57f5,0xf652b8bd,0x0f86b0f3,0xfb3a088a,0x4dc71533,0x7b3617d2,0xa34e87eb,0xba2a9bdd,0xe3381306,0x14bad6bb,0xc96dc7c2,0x333b54b6,0x9be47cfa,0x1dcf9299,0xe7ea5f99,0xb38feacd,0xc3cfe2f7,0x5b87e822,0x39c5ab56,0x18f4a18f,0x2d484d9c,0x4163d519,0x79769e98,0xf58a67f0,0x40590c02,0x319671c0,0x266b133a,0xaf81b287,0x6a31f737,
    0xe3bc0197,0x55079913,0x9f72c696,0x363e00c8,0x53153947,0xebfd127f,0x00f60519,0x46a6b62a,0x93b83380,0x3fe29324,0xdfc67091,0x0f62386d,0xdc375e79,0x8fea3f3e,0xdf8463d0,0x3702fa7b,0x3954435e,0x87caa648,0xa9158bee,0x08f30c25,0x66b82936,0xe7fc3feb,0x183c5450,0xd7ef4345,0x798c7963,0xc02cf557,0x098553d1,0xfa4312aa,0xe29ef883,0x7caf128d,0x74b3a07d
    };

/* Hash up to STRIPE bytes, consisting of whole UINT pairs, including optional
   UINT pair in the extra[] */
static UINT farsh_pairs (const UINT *data, size_t elements, const UINT* extra, const UINT *farsh_key)
{
    ULONG sum = 0;  int i;
    for (i=0; i < elements; i+=2)
        sum += (data[i] + farsh_key[i]) * (ULONG)(data[i+1] + farsh_key[i+1]);
    if (extra)
        sum += (extra[0] + farsh_key[i]) * (ULONG)(extra[1] + farsh_key[i+1]);
    return (UINT)(sum >> 32);
}

/* Hash up to STRIPE bytes, with special optimization for exactly STRIPE input
   bytes and careful handling of partial UINT pair at the end of buffer */
static UINT farsh_block (const void *data_ptr, size_t bytes, const UINT *farsh_key)
{
    const UINT *data = (const UINT*) data_ptr;
    if (bytes == STRIPE)  return farsh_pairs (data, STRIPE_ELEMENTS, NULL, farsh_key);
    size_t elements = (bytes/sizeof(UINT)) & (~1);
    UINT extra_data[2] = {0};
    size_t extra_bytes = bytes - elements*sizeof(UINT);
    memcpy (extra_data, data+elements, extra_bytes);
    return farsh_pairs (data, elements, extra_bytes?extra_data:NULL, farsh_key);
}

/* Hash the buffer of arbitrary size */
UINT farsh_keyed (const void *data_ptr, size_t bytes, const UINT *farsh_key)
{
    ULONG sum = 0;  int i = 0;
    const char *data = (const char*) data_ptr,  *end = data+bytes;
    while (data < end)
    {
        size_t minbytes = (bytes<STRIPE? bytes : STRIPE);
        UINT h = farsh_block (data, minbytes, farsh_key);
        data += minbytes;
        sum += h*(ULONG)farsh_key[i];
        if (++i == STRIPE_ELEMENTS)
            i = 0;/*to do*/
    }
    return (UINT)(sum >> 32);
}

/* Hash the buffer of arbitrary size */
void farsh_keyed_n (const void *data_ptr, size_t bytes, const UINT *farsh_key, int n, UINT *hash)
{
    int i;
    for (i=0; i < n; i++)
        hash[i] = farsh_keyed (data_ptr, bytes, farsh_key+i);
}

/* Hash the buffer of arbitrary size */
void farsh_n (const void *data_ptr, size_t bytes, int n, UINT *hash)
{
    farsh_keyed_n (data_ptr, bytes, farsh_key, n, hash);
}

/* Hash the buffer of arbitrary size */
UINT farsh (const void *data_ptr, size_t bytes)
{
    return farsh_keyed (data_ptr, bytes, farsh_key);
}

#undef STRIPE
#undef STRIPE_ELEMENTS
#undef UINT
#undef ULONG
