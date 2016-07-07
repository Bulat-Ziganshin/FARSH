/* Allows to measure the time required to execute XXX() in the following way:

    Timer t;
    t.Start();
    XXX();
    t.Stop();
    double seconds = t.Elapsed();
*/

#pragma once


#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)

#include <windows.h>

struct Timer
{
    Timer()
    {
        // Initialize the resolution of the timer
        if (!QueryPerformanceFrequency(&m_freq))
        {
            printf("QueryPerformanceFrequency failed!\n");
        }

        // Calculate the overhead of the timer in ticks
        QueryPerformanceCounter(&m_start);
        QueryPerformanceCounter(&m_stop);
        m_overhead = m_stop.QuadPart - m_start.QuadPart;
    }

    void Start()
    {
        QueryPerformanceCounter(&m_start);
    }

    void Stop()
    {
        QueryPerformanceCounter(&m_stop);
    }

    // Returns elapsed time in seconds
    double Elapsed()
    {
        return (m_stop.QuadPart - m_start.QuadPart - m_overhead) / double(m_freq.QuadPart);
    }

private:

    LARGE_INTEGER m_start;
    LARGE_INTEGER m_stop;
    LARGE_INTEGER m_freq;
    LONGLONG m_overhead;
};


#else // this should handle any Unixes

#include <sys/time.h>

struct Timer
{
    Timer()
    {
        // Calculate the timer overhead
        overhead = 0;
        Start();
        Stop();
        overhead = Elapsed();
    }

    void Start()
    {
        gettimeofday (&timerStart, NULL);
    }

    void Stop()
    {
        gettimeofday (&timerStop, NULL);
    }

    // Returns elapsed time in seconds
    double Elapsed()
    {
        struct timeval timerElapsed;
        timersub (&timerStop, &timerStart, &timerElapsed);
        return (timerElapsed.tv_sec + timerElapsed.tv_usec/1e6 - overhead);
    }

private:

    struct timeval timerStart, timerStop;
    double overhead;
};

#endif
