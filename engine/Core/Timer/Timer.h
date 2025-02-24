/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Types/String.h>
#include <chrono>


class Timer 
{
public:
    Timer();

    void Start();

    void Stop();

    void Reset();

    double getElapsedMilliseconds() const;

    double getElapsedSeconds() const;

    bool isRunning() const;

    static String Now(const String Format = L"%yyyy-mm-dd %H:%M:%S");
private:
#ifdef _WIN32
    double cpuFrequency = 0.0;

    uint64 clock = 0;
#endif // _WIN32

    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

    bool bRunning;
};
