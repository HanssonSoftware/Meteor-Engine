/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <stdint.h>

#include "Core.proxy.h"

#ifdef MR_DEBUG
static constexpr bool bIsRunningDebugMode = true;
#else
static constexpr bool bIsRunningDebugMode = false;
#endif // MR_DEBUG

class String;
struct LogDescriptor;
struct LogAssertion;
class IFile;



struct LogEntry {};

class CORE_API ILogger
{
public:
	static ILogger* Get();

	template<typename T>
	static T* Get();

	virtual void Initialize();

	virtual void Shutdown() {};

	const LogDescriptor* GetActualEntry() const { return Get()->actualDescriptor; };

	void SetActualLog(const LogDescriptor* newDescriptor) { actualDescriptor = newDescriptor; }

	virtual void ProcessMessage(const LogDescriptor* Descriptor) {};

	virtual void ProcessAssertion(const LogAssertion* Info) {};

	virtual bool IsDebuggerAttached() { return false; };

	virtual void SendToOutputBuffer(const String* Buffer) {};

	virtual void HandleFatal() {};
protected:
	static constexpr const wchar_t* FormatSeverity(uint8_t Severity) noexcept;

	ILogger() = default;

	virtual ~ILogger() noexcept
	{
		Shutdown();
	};

	const LogDescriptor* actualDescriptor = nullptr;
		
	bool bIsUsingVerbose = false;

	bool bIsInitialized = false;

	static inline ILogger* object;
};

#include "LogMacros.h"