/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/PlatformDefs.h>
#include <Common/Delegate.h>

struct ThreadTaskDescriptor
{
	/*Delegate<> Task; */

	bool bRunOnlyOnce = false;
};

class IThread
{
public:
	IThread();

	virtual ~IThread() = default;

	virtual void Start() = 0;

	virtual void Stop() = 0;

	virtual void Pause() = 0;

	inline void Continue() { Start(); }; 

	bool IsRunning() const { return bRunning; };

protected:
	uint32 id = 0;

	bool bRunning = false;
};

