/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Log.h"
#include <Commandlet.h>
#include <MemoryManager.h>

//#include <Platform/File.h>
//#include <Platform/Paths.h>
//#include <Platform/PlatformLayout.h>
//#include <Platform/Timer.h>
//#include <Platform/FileManager.h>
//
//#ifdef MR_PLATFORM_WINDOWS
//#include <Windows/WindowsLog.h>
//#else
//
//#endif // MR_PLATFORM_WINDOWS

LOG_ADDCATEGORY(Standard);

ILogger* ILogger::Get()
{
	if (object == nullptr)
	{
		object = /*MemoryManager::Get().Allocate<ILogger>(sizeof(ILogger))*/new ILogger;
	}

    return object;
}

template<typename T>
T* ILogger::Get()
{
	if (object == nullptr)
	{
		object = /*MemoryManager::Get().Allocate<T>(sizeof(T))*/new T;
	}

    return (T*)object;
}

void ILogger::Initialize()
{
    bIsUsingVerbose = Commandlet::Parse("-verbose", nullptr);
}

//{
//    SetActualLog(Descriptor);
//
//    String fullMessage;
//    const String current = Timer::Format("yyyy-MM-dd HH:mm:ss");
//
//    if (Descriptor->severity == Fatal)
//    {
//        fullMessage = String::Format(
//            "=============[ Fatal error ]=============\nWhere:\t\t%ls\nWhen:\t\t%ls\nMessage:\t%ls\n\nFile:\t%ls\n",
//            Descriptor->function,
//            *current,
//            *Descriptor->message,
//            Descriptor->file);
//    }
//    else
//    {
//        fullMessage = String::Format("[%ls] %ls: %ls\n", 
//            current.Chr(),
//            Descriptor->team,
//            *Descriptor->message);
//    }
//
//    SendToOutputBuffer(&fullMessage);
//}

constexpr const wchar_t* ILogger::FormatSeverity(uint8_t Severity) noexcept
{
	switch (Severity)
	{
	case Log:
		return L"Log";
	case Warn:
		return L"Warning";
	case Error:
		return L"Error";
	case Fatal:
		return L"Fatal";
	case Verbose:
		return L"Verbose";
	}

	return L"";
}


void LogDescriptor::SetMessage(const wchar_t* message, ...)
{
    va_list d = nullptr;
    va_start(d, message);
    const int reqAmount = vswprintf(nullptr, 0, message, d);

    wchar_t* big = MemoryManager::Get().Allocate<wchar_t>(reqAmount + 1);
    vswprintf(big, reqAmount + 1, message, d);
    va_end(d);

    big[reqAmount] = L'\0';

    this->message = big;
    MemoryManager::Get().Deallocate(big);
}
