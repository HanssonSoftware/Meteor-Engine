/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>
#include "ThreadDescriptor.h"

class IThread
{
	IThread() = default;

	virtual void Init() = 0;

	virtual void Shutdown() = 0;

	virtual void Pause() = 0;

	virtual void Resume() = 0;

protected:
	bool bIsPaused = false;

	int id = -1;
};

