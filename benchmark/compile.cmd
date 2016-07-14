gcc -O3 -funroll-loops -s -static -m32 -march=pentium3     main.cpp -oaligned-farsh-x86         -DFARSH_ALIGNED_INPUT
gcc -O3 -funroll-loops -s -static -m32 -msse2 -DFARSH_SSE2 main.cpp -oaligned-farsh-x86-sse2    -DFARSH_ALIGNED_INPUT
gcc -O3 -funroll-loops -s -static -m32 -mavx2 -DFARSH_AVX2 main.cpp -oaligned-farsh-x86-avx2    -DFARSH_ALIGNED_INPUT
gcc -O3 -funroll-loops -s -static -m64                     main.cpp -oaligned-farsh-x64-nosimd  -DFARSH_ALIGNED_INPUT
gcc -O3 -funroll-loops -s -static -m64 -msse2 -DFARSH_SSE2 main.cpp -oaligned-farsh-x64         -DFARSH_ALIGNED_INPUT
gcc -O3 -funroll-loops -s -static -m64 -mavx2 -DFARSH_AVX2 main.cpp -oaligned-farsh-x64-avx2    -DFARSH_ALIGNED_INPUT
gcc -O3 -funroll-loops -s -static -m32 -march=pentium3     main.cpp -ofarsh-x86
gcc -O3 -funroll-loops -s -static -m32 -msse2 -DFARSH_SSE2 main.cpp -ofarsh-x86-sse2
gcc -O3 -funroll-loops -s -static -m32 -mavx2 -DFARSH_AVX2 main.cpp -ofarsh-x86-avx2
gcc -O3 -funroll-loops -s -static -m64                     main.cpp -ofarsh-x64-nosimd
gcc -O3 -funroll-loops -s -static -m64 -msse2 -DFARSH_SSE2 main.cpp -ofarsh-x64
gcc -O3 -funroll-loops -s -static -m64 -mavx2 -DFARSH_AVX2 main.cpp -ofarsh-x64-avx2
