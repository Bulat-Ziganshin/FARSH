set GCC=gcc -O3 -funroll-loops -s -static -c ../farsh.c
::  -g
%GCC% -O3 -funroll-loops -m32               -Wa,-adhlns=gcc-x86.lst
%GCC% -O3 -funroll-loops -m32 -msse2 -DSSE2 -Wa,-adhlns=gcc-x86-sse2.lst
%GCC% -O3 -funroll-loops -m32 -mavx2 -DAVX2 -Wa,-adhlns=gcc-x86-avx2.lst
%GCC% -O3 -funroll-loops -m64 -msse2 -DSSE2 -Wa,-adhlns=gcc-x64.lst
%GCC% -O3 -funroll-loops -m64 -mavx2 -DAVX2 -Wa,-adhlns=gcc-x64-avx2.lst
