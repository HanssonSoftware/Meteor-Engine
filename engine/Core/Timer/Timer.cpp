/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Timer.h"
#include <sstream>
#include <iomanip>
#include <Types/String.h>

Timer::Timer() : 
    bRunning(false) 
{

}

void Timer::Start()
{
    startTime = std::chrono::high_resolution_clock::now();
    bRunning = true;
}

void Timer::Stop() 
{
    endTime = std::chrono::high_resolution_clock::now();
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
        endTimePoint = endTime;
    }

    return std::chrono::duration<double, std::milli>(endTimePoint - startTime).count();
}

double Timer::getElapsedSeconds() const 
{
    return getElapsedMilliseconds() / 1000.0;
}

bool Timer::isRunning() const 
{
    return bRunning;
}

String Timer::Now(String Format)
{
    time_t rawtime;
    wchar_t buffer[256];

    time(&rawtime);

    tm* a = localtime(&rawtime);

    wcsftime(buffer, 256, Format.Chr(), a);



    return buffer;
}

