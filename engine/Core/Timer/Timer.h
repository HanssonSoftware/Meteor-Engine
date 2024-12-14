/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

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

    static String Now(String Format = L"%Y-%m-%d %H:%M:%S");
private:

    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

    bool bRunning;
};
