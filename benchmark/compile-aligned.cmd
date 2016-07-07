gcc -O3 -funroll-loops -s -static -m32                     main.cpp -ofarsh-x86         -DFARSH_ALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m32 -msse2 -DFARSH_SSE2 main.cpp -ofarsh-x86-sse2    -DFARSH_ALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m32 -mavx2 -DFARSH_AVX2 main.cpp -ofarsh-x86-avx2    -DFARSH_ALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m64                     main.cpp -ofarsh-x64-nosimd  -DFARSH_ALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m64 -msse2 -DFARSH_SSE2 main.cpp -ofarsh-x64         -DFARSH_ALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m64 -mavx2 -DFARSH_AVX2 main.cpp -ofarsh-x64-avx2    -DFARSH_ALIGNED_DATA
