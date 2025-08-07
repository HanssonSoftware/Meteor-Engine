/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>

class String;

struct Time
{
    uint32 year = 0;
    uint32 month = 0;
    uint32 day = 0;
    uint32 hour = 0;
    uint32 minute = 0;
    uint32 second = 0;
    uint32 millisecond = 0;
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
    uint64 begin = 0;

    uint64 end = 0;

    bool bRunning = false;
};

#include "PlatformLayout.h"

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/WindowsTimer.h>
#endif // MR_PLATFORM_WINDOWS
