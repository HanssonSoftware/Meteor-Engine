/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Logging/Log.h>
#include <stdint.h>

class IFile;

class WindowsLogger : public ILogger
{
	friend class ILogger;
public:
	virtual ~WindowsLogger() noexcept;

	virtual void Initialize() override;

	virtual void Shutdown() override;

	virtual void HandleFatal() override;

	//int32_t TransmitAssertion(LogAssertion& Info);

	virtual void SendToOutputBuffer(const String& Buffer) override;

	static inline bool IsDebuggerAttached();

protected:
	WindowsLogger();

	IFile* consoleBuffer = nullptr;
};