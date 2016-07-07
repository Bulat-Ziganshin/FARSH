// //////////////////////////////////////////////////////////
// timing.c
// Copyright (c) 2014 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#pragma once

// include OS specific timing library
#ifdef _WIN32  // Windows
#include <windows.h>
#else         // Linux
#include <time.h>
#endif


/// return a timestamp with sub-second precision
/** QueryPerformanceCounter and clock_gettime have an undefined starting point (null/zero)
    and can wrap around, i.e. be nulled again. **/
inline double seconds()
{
#ifdef _WIN32  // Windows
  static LARGE_INTEGER frequency;
  if (frequency.QuadPart == 0)
    ::QueryPerformanceFrequency(&frequency);

  LARGE_INTEGER now;
  ::QueryPerformanceCounter(&now);
  return now.QuadPart / double(frequency.QuadPart);
#else         // Linux
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec + now.tv_nsec / 1000000000.0;
#endif
}
