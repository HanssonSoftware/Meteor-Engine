/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Platform/Timer.h>
#include <Windows/Windows.h>

class WindowsTimer : public ITimer
{
public:
	WindowsTimer();

	virtual ~WindowsTimer() = default;

	virtual void Start() override;

	virtual void Stop() override;
	
	virtual void Reset() override;

	static Time Now() noexcept;
private:
	HANDLE timer;
};

using Timer = WindowsTimer;