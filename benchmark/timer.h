//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corp.
//
// File: timer.h
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED WARRANTIES OR
// CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
//
// See the Apache Version 2.0 License for specific language governing permissions and
// limitations under the License.
//--------------------------------------------------------------------------------------

#pragma once
#include <windows.h>

struct Timer
{
    void Start()
    {
        QueryPerformanceCounter(&m_start);
    }

    void Stop()
    {
        QueryPerformanceCounter(&m_stop);
    }

    // Returns elapsed time in milliseconds (ms)
    double Elapsed()
    {
        return (m_stop.QuadPart - m_start.QuadPart - m_overhead) \
                                          * 1000.0 / m_freq.QuadPart;
    }

    Timer()
    {
        // Initialize the resolution of the timer
        QueryPerformanceFrequency(&m_freq);

        // Calculate the overhead of the timer
        m_overhead = Timer::GetOverhead();
    }

private:

    // Returns the overhead of the timer in ticks
    LONGLONG GetOverhead()
    {
        Start();
        Stop();
        return m_stop.QuadPart - m_start.QuadPart;
    }

    LARGE_INTEGER m_start;
    LARGE_INTEGER m_stop;
    LARGE_INTEGER m_freq;
    LONGLONG m_overhead;
};
