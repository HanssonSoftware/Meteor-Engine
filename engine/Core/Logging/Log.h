/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "LogHelpers.h"


#ifdef MR_DEBUG
static constexpr const bool bIsRunningDebugMode = true;
#else
static constexpr const bool bIsRunningDebugMode = false;
#endif // MR_DEBUG

struct String;
struct LogAssertion;
class IFile;

struct LogEntry
{

};

struct ILogger
{
	static void Initialize();

	static void Shutdown();

	virtual ~ILogger() noexcept;

	static ILogger* Get();

	static LogDescriptor* GetActualEntry() { return Get()->actualDescriptor; };

	static void HandleFatal();

	static void SetActualLog(LogDescriptor* newDescriptor);

	static void TransmitMessage(LogDescriptor * Descriptor);

	static void TransmitAssertion(const LogAssertion* Info);

	static inline bool IsDebuggerAttached();

	virtual void SendToOutputBuffer(const String Buffer);
protected:
	ILogger();

	LogDescriptor* actualDescriptor = nullptr;
		
	bool bIsUsingVerbose = false;

	bool bIsInitialized = false;

	bool bIsUsingFile = true;

	IFile* buffer = nullptr;
};

#include <Platform/PlatformLayout.h>

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/WindowsLog.h>
#else

#endif // MR_PLATFORM_WINDOWS
