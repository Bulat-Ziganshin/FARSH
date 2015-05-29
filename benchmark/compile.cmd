set GCC=gcc -O3 -funroll-loops -s -static
%GCC% -m32               main.cpp -ofarsh-x86
%GCC% -m32 -msse2 -DSSE2 main.cpp -ofarsh-x86-sse2
%GCC% -m32 -mavx2 -DAVX2 main.cpp -ofarsh-x86-avx2
%GCC% -m64 -msse2 -DSSE2 main.cpp -ofarsh-x64
%GCC% -m64 -mavx2 -DAVX2 main.cpp -ofarsh-x64-avx2
