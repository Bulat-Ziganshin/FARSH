set GCC=gcc -O3 -funroll-loops -s -static
%GCC% -m32               main.cpp -ofarsh32.exe
%GCC% -m32 -msse2 -DSSE2 main.cpp -ofarsh32sse2.exe
%GCC% -m64 -msse2 -DSSE2 main.cpp -ofarsh64.exe
%GCC% -m64 -mavx2 -DAVX2 main.cpp -ofarsh64avx2.exe
%GCC% -m32 -mavx2 -DAVX2 main.cpp -ofarsh32avx2.exe
