/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Timer.h"
//#include <sstream>
#include <iomanip>
#include <Types/String.h>
//#include <Windows.h>
//#include <Log/Exception.h>
//#include <Platform/Log.h>

Timer::Timer()
{


}

String Timer::Now(const String Format)
{
    if (Format.IsEmpty())
        return "";

    time_t rawtime;
    char buffer[256];

    time(&rawtime);

    tm* a = localtime(&rawtime);

    strftime(buffer, 256, Format.Chr(), a);

    return buffer;
}

