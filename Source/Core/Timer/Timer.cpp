/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#include "Timer.h"
#include <sstream>
#include <iomanip>

Timer::Timer() : 
    bRunning(false) 
{

}

void Timer::Start() {
    startTime = std::chrono::high_resolution_clock::now();
    bRunning = true;
}

void Timer::Stop() {
    endTime = std::chrono::high_resolution_clock::now();
    bRunning = false;
}

void Timer::Reset() {
    bRunning = false;
}

double Timer::GetElapsedMilliseconds() const {
    std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint;
    if (bRunning) {
        endTimePoint = std::chrono::high_resolution_clock::now();
    }
    else {
        endTimePoint = endTime;
    }
    return std::chrono::duration<double, std::milli>(endTimePoint - startTime).count();
}

double Timer::GetElapsedSeconds() const {
    return GetElapsedMilliseconds() / 1000.0;
}

bool Timer::IsRunning() const {
    return bRunning;
}

std::wstring Timer::Now(const wchar_t* Format)
{
    time_t rawtime;
    wchar_t buffer[20];

    time(&rawtime);

    struct tm a;
    localtime_s(&a, &rawtime);

    wcsftime(buffer, 20, Format, &a);
    return buffer;
}

