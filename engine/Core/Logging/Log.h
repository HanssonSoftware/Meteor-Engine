/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once

#ifdef MR_DEBUG
static constexpr const bool bIsRunningDebugMode = true;
#else
static constexpr const bool bIsRunningDebugMode = false;
#endif // MR_DEBUG

class String;
struct LogAssertion;
class IFile;

struct LogEntry {};

enum LogSeverity : short
{
	Log, /** Logging standard, you will use this as most. */
	Verbose, /** Ctrl+C, Ctrl+V from Info. Also this message won't be saved, only for console screen.*/
	Warn, /** User warning type, text color is different, to draw user attention to console screen.*/
	Error, /** Indicating an error, which needs some attention.*/
	Fatal /** Very dangerous situation, this will terminate the app.*/
};


struct LogDescriptor
{
	LogDescriptor() = delete;

	LogDescriptor(const char* entry, LogSeverity severity, const char* Message, const char* function, const char* file, ...) noexcept;

	const char* team;

	LogSeverity severity;

	String message;

	const char* function;

	const char* file;
};



class ILogger
{
public:
	static ILogger* Get();

	void Initialize();

	void Shutdown();

	virtual ~ILogger() noexcept;

	LogDescriptor* GetActualEntry() { return Get()->actualDescriptor; };

	void SetActualLog(LogDescriptor* newDescriptor);

	virtual void TransmitMessage(LogDescriptor* Descriptor);

	virtual void TransmitAssertion(const LogAssertion* Info);

	inline bool IsDebuggerAttached();

	virtual void SendToOutputBuffer(const String& Buffer);

protected:
	ILogger();

	void HandleFatal();

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


#include "LogMacros.inl"