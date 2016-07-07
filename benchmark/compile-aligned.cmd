gcc -O3 -funroll-loops -s -static -m32               main.cpp -ofarsh-x86         -DALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m32 -msse2 -DSSE2 main.cpp -ofarsh-x86-sse2    -DALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m32 -mavx2 -DAVX2 main.cpp -ofarsh-x86-avx2    -DALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m64               main.cpp -ofarsh-x64-nosimd  -DALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m64 -msse2 -DSSE2 main.cpp -ofarsh-x64         -DALIGNED_DATA
gcc -O3 -funroll-loops -s -static -m64 -mavx2 -DAVX2 main.cpp -ofarsh-x64-avx2    -DALIGNED_DATA
