/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Platform/PlatformDefs.h>

//struct String;

class Timer 
{
public:
    Timer();

    virtual ~Timer() = default;

    virtual void Start() = 0;

    virtual void Stop() = 0;

    virtual void Reset() = 0;

    bool IsRunning() const { return bRunning; };

    static String Now(const String Format = "%Y-%m-%d %H:%M:%S");

protected:
    uint64 begin = 0;

    uint64 end = 0;

    bool bRunning = false;
};
