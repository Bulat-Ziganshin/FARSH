@echo off
set options=main.cpp
set options_ms=-MP -Gy -GL -GR- -nologo %options% user32.lib shell32.lib ole32.lib advapi32.lib %* -link -LARGEADDRESSAWARE
set options_ms_cl=-O2 -GL -Gy -EHsc %options_ms%
set options_ms_icl=-w -O3 -Qipo %options_ms%
set options_ms_x86=-MACHINE:x86 -SUBSYSTEM:CONSOLE,5.01
set options_ms_x64=-MACHINE:x64 -SUBSYSTEM:CONSOLE,5.02

gcc -std=c++11 -O3 -m32 -s -static -msse2 %options% -ofarsh32.exe
gcc -std=c++11 -O3 -m64 -s -static -msse2 %options% -ofarsh64.exe

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
cl -Fefarsh32m.exe -arch:SSE2 %options_ms_cl% %options_ms_x86%

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
cl -Fefarsh64m.exe %options_ms_cl% %options_ms_x64%

call "C:\Program Files (x86)\Intel\Composer XE 2013 SP1\bin\ipsxe-comp-vars.bat" ia32
icl -Fefarsh32i.exe -arch:SSE2 %options_ms_icl% %options_ms_x86%
iccpatch.exe farsh32i.exe >nul

call "C:\Program Files (x86)\Intel\Composer XE 2013 SP1\bin\ipsxe-comp-vars.bat" intel64
icl -Fefarsh64i.exe %options_ms_icl% %options_ms_x64%
iccpatch.exe farsh64i.exe >nul

del *.exe.bak *.obj *.res >nul 2>nul
