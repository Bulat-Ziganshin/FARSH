::set DEBUG=-g
gcc -O3 -funroll-loops -m32               %DEBUG% -Wa,-adhlns=gcc32.lst     main.cpp
gcc -O3 -funroll-loops -m32 -msse2 -DSSE2 %DEBUG% -Wa,-adhlns=gcc32sse2.lst main.cpp
gcc -O3 -funroll-loops -m64 -msse2 -DSSE2 %DEBUG% -Wa,-adhlns=gcc64sse2.lst main.cpp
gcc -O3 -funroll-loops -m64 -mavx2 -DAVX2 %DEBUG% -Wa,-adhlns=gcc64avx2.lst main.cpp
gcc -O3 -funroll-loops -m32 -mavx2 -DAVX2 %DEBUG% -Wa,-adhlns=gcc32avx2.lst main.cpp
