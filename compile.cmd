@echo off
set options=
set options_ms=-MP -Gy -GL -GR- -nologo %options% user32.lib shell32.lib ole32.lib advapi32.lib %* -link -LARGEADDRESSAWARE
set options_ms_cl=-Ox -GL -Gy -EHsc %options_ms%
set options_ms_icl=-w -O3 -Qipo %options_ms%
set options_ms_x86=-MACHINE:x86
set options_ms_x64=-MACHINE:x64

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
cl -Fefarsh32.exe %options_ms_cl% %options_ms_x86%

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
cl -Fefarsh64.exe %options_ms_cl% %options_ms_x64%

::call "C:\Program Files (x86)\Intel\Parallel Studio 2011\ips-vars.cmd" ia32_intel64
::icl -Fesrep64i.exe %options_ms_icl% %options_ms_x64%

del *.exe.bak *.obj *.res >nul 2>nul
