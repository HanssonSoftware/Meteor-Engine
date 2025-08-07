/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Logging/Log.h>

class IFile;

struct WindowsLog : public ILogger
{
	WindowsLog();

	virtual ~WindowsLog() noexcept;

	static void Initialize();

	static void Shutdown();

	static inline bool IsDebuggerAttached();

	static void HandleFatal();

	static int TransmitAssertion(LogAssertion& Info);

	virtual void SendToOutputBuffer(const String& Buffer) override;

protected:
	IFile* consoleBuffer = nullptr;
};


using Logger = WindowsLog;