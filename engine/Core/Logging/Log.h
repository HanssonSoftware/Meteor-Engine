/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <stdint.h>

//#include "Core.proxy.h"

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

class /*CORE_API*/ ILogger
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
protected:
	static constexpr const char* FormatSeverity(uint8_t Severity) noexcept;

	ILogger() = default;

	virtual ~ILogger() noexcept
	{
		Shutdown();
	};

	const LogDescriptor* actualDescriptor = nullptr;
		
	bool bIsUsingVerbose = false;

	bool bIsInitialized = false;


};

#include "LogMacros.h"