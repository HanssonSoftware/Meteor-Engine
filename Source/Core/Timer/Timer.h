/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#pragma once
#include <chrono>

class Timer {
public:
    Timer();

    void Start();

    void Stop();

    void Reset();

    double GetElapsedMilliseconds() const;

    double GetElapsedSeconds() const;

    bool IsRunning() const;

    static std::wstring Now(const wchar_t* Format = L"%Y-%m-%d %H:%M:%S");
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    bool bRunning;
};
