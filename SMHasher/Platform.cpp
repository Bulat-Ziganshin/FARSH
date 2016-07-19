#include "Platform.h"

#include <stdio.h>

void testRDTSC ( void )
{
  int64_t temp = rdtsc();

  printf("%d",(int)temp);
}

#if defined(_WIN32)

#include <windows.h>

static DWORD_PTR process_mask = 1, system_mask = 1;
int thread_priority = THREAD_PRIORITY_NORMAL;

void SetAffinity ( int cpu )
{
  GetProcessAffinityMask(GetCurrentProcess(), &process_mask, &system_mask);   // i don't know why, but it can't fetch the process mask as set by the "start" command
  thread_priority = GetThreadPriority (GetCurrentThread ());

  SetThreadAffinityMask(GetCurrentThread(),cpu);
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
}

void ResetAffinity()
{
  SetThreadAffinityMask(GetCurrentThread(),process_mask);
  SetThreadPriority(GetCurrentThread(), thread_priority);
}

#else

#include <sched.h>

void SetAffinity ( int /*cpu*/ )
{
#if !defined(__CYGWIN__) && !defined(__APPLE__)
  cpu_set_t mask;

  CPU_ZERO(&mask);

  CPU_SET(2,&mask);

  if( sched_setaffinity(0,sizeof(mask),&mask) == -1)
  {
    printf("WARNING: Could not set CPU affinity\n");
  }
#endif
}

void ResetAffinity()
{
  // #TODO
}

#endif
