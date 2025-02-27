/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Timer.h"
#include <sstream>
#include <iomanip>
#include <Types/String.h>
#include <Windows.h>
#include <Log/Exception.h>
#include <Log/Log.h>

Timer::Timer() : 
    bRunning(false) 
{


}

void Timer::Start()
{
#ifdef _WIN32
    LARGE_INTEGER lg;
    if (QueryPerformanceFrequency(&lg) == 0)
        THROW_EXCEPTION("QueryPerformanceFrequency Error!");

    cpuFrequency = lg.QuadPart / 1000.0;

    if (QueryPerformanceCounter(&lg) == 0)
        THROW_EXCEPTION("QueryPerformanceCounter Error!");

    clock = lg.QuadPart;
#endif // _WIN32
    bRunning = true;
}

void Timer::Stop() 
{
    //endTime = std::chrono::high_resolution_clock::now();
    bRunning = false;
}

void Timer::Reset() 
{
    bRunning = false;
}

double Timer::getElapsedMilliseconds() const 
{
    std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint;
    if (bRunning) 
    {
        endTimePoint = std::chrono::high_resolution_clock::now();
    }
    else 
    {
        //endTimePoint = endTime;
    }

    return 1;
    //return std::chrono::duration<double, std::milli>(endTimePoint - startTime).count();
}

double Timer::getElapsedSeconds() const 
{
    return getElapsedMilliseconds() / 1000.0;
}

bool Timer::isRunning() const 
{
    return bRunning;
}

String Timer::Now(const String Format)
{
    if (Format.isEmpty())
        return "";

    time_t rawtime;
    wchar_t buffer[256];

    time(&rawtime);

    tm* a = localtime(&rawtime);

    wcsftime(buffer, 256, Format.Chr(), a);

    return buffer;
}

