gcc -O3 -funroll-loops -s -static -m32               main.cpp -ofarsh-x86
gcc -O3 -funroll-loops -s -static -m32 -msse2 -DSSE2 main.cpp -ofarsh-x86-sse2
gcc -O3 -funroll-loops -s -static -m32 -mavx2 -DAVX2 main.cpp -ofarsh-x86-avx2
gcc -O3 -funroll-loops -s -static -m64 -msse2 -DSSE2 main.cpp -ofarsh-x64
gcc -O3 -funroll-loops -s -static -m64 -mavx2 -DAVX2 main.cpp -ofarsh-x64-avx2
