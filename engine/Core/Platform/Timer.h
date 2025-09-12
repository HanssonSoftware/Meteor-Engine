/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <stdint.h>

class String;

struct Time
{
    uint32_t year = 0;
    uint32_t month = 0;
    uint32_t day = 0;
    uint32_t hour = 0;
    uint32_t minute = 0;
    uint32_t second = 0;
    uint32_t millisecond = 0;
};

class ITimer 
{
public:
    ITimer() = default;

    virtual ~ITimer() = default;

    virtual void Start() = 0;

    virtual void Stop() = 0;

    virtual void Reset() = 0;

    bool IsRunning() const { return bRunning; };

    static Time Now() noexcept;

protected:
    uint64_t begin = 0;

    uint64_t end = 0;

    bool bRunning = false;
};

#include "PlatformLayout.h"

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/WindowsTimer.h>
#endif // MR_PLATFORM_WINDOWS
