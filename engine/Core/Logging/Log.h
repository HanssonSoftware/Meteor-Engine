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
	LogDescriptor() = default;

	LogDescriptor(const char* entry, LogSeverity severity, const char* Message, const char* function, const char* file, ...) noexcept;

	constexpr void SetValues(const char* category, LogSeverity severity, const char* function, const char* file, const int line);

	const char* team;

	LogSeverity severity;

	const char* message;

	const char* function;

	const char* file;

	int line;
};



class ILogger
{
	friend class WindowsLogger;
public:
	static ILogger* Get();

	virtual void Initialize();

	virtual void Shutdown();

	virtual ~ILogger() noexcept;

	LogDescriptor* GetActualEntry() { return Get()->actualDescriptor; };

	void SetActualLog(LogDescriptor* newDescriptor);

	virtual void TransmitMessage(LogDescriptor* Descriptor);

	virtual void TransmitAssertion(const LogAssertion* Info);

	static inline bool IsDebuggerAttached();

	virtual void SendToOutputBuffer(const String& Buffer);

	virtual void HandleFatal();

protected:
	ILogger();

	LogDescriptor* actualDescriptor = nullptr;
		
	bool bIsUsingVerbose = false;

	bool bIsInitialized = false;

	bool bIsUsingFile = true;

	IFile* buffer = nullptr;
};

#include "LogMacros.inl"