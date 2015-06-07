gcc -O3 -funroll-loops -s -static -c ../farsh.c -m32               -Wa,-adhlns=gcc-x86.lst
gcc -O3 -funroll-loops -s -static -c ../farsh.c -m32 -msse2 -DSSE2 -Wa,-adhlns=gcc-x86-sse2.lst
gcc -O3 -funroll-loops -s -static -c ../farsh.c -m32 -mavx2 -DAVX2 -Wa,-adhlns=gcc-x86-avx2.lst
gcc -O3 -funroll-loops -s -static -c ../farsh.c -m64               -Wa,-adhlns=gcc-x64-nosimd.lst
gcc -O3 -funroll-loops -s -static -c ../farsh.c -m64 -msse2 -DSSE2 -Wa,-adhlns=gcc-x64.lst
gcc -O3 -funroll-loops -s -static -c ../farsh.c -m64 -mavx2 -DAVX2 -Wa,-adhlns=gcc-x64-avx2.lst
