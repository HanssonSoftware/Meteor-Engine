/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WindowsTimer.h"
#include "Windows/Windows.h"
#include <Logging/LogMacros.h>

LOG_ADDCATEGORY(Timer);

WinTimer::WinTimer()
{
	timer = CreateWaitableTimer(0, /*TRUE*/ 1, 0);
	if (timer == 0)
	{
		MR_LOG(LogTimer, Error, "CreateWaitableTimer failed!");
		return;
	}
}

void WinTimer::Start()
{
	bRunning = true;

	//SetWaitableTimer(timer, 0, 0, 0, 0, 0);
}

void WinTimer::Stop()
{
	bRunning = false;

	if (timer)
	{
		CloseHandle(timer);
		timer = 0;
	}
}

void WinTimer::Reset()
{
}
