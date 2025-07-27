/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Platform/Timer.h>

class WinTimer : public Timer
{
public:
	WinTimer();

	virtual void Start() override;

	virtual void Stop() override;
	
	virtual void Reset() override;

private:
	void* /*HANDLE*/ timer;
};

