/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once

#ifdef MR_DEBUG
static constexpr const bool bIsRunningDebugMode = true;
#else
static constexpr const bool bIsRunningDebugMode = false;
#endif // MR_DEBUG

class String;
struct LogDescriptor;
struct LogAssertion;
class IFile;


struct LogEntry {};

class ILogger
{
	friend class WindowsLogger;
public:
	static ILogger* Get();

	virtual void Initialize();

	virtual void Shutdown();

	virtual ~ILogger() noexcept;

	const LogDescriptor* GetActualEntry() const { return Get()->actualDescriptor; };

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

	bool bHasConsoleWindow = false;

	bool bIsUsingFile = true;

	IFile* buffer = nullptr;
};

#include "LogMacros.h"