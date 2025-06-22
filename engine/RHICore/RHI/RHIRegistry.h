/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Vector.h>
#include <RHI/RHICommons.h>
#include <vector>

struct String;
class IRHIOutputContext;

/*
* Knows everything about your graphics setup so you don't have to.
	Keeps track of available monitors and where the app is running.
	Figures out if you're on DX, Vulkan, or whatever.
	Lets command-line arguments mess with settings.
*/
class IRHIRegistry
{
public:
	/** Finds your currently viewed monitor, by your seleceted RHI. If you get -1 that means the function is not implemented properly. */
	virtual int GetMonitorIndex() const = 0;

	virtual String GetName() const = 0;

	virtual bool Initialize() = 0;

	virtual void CleanUp() const = 0;

	virtual IRHIOutputContext* GetOutputContext() const
	{
		return context;
	}

  	virtual ~IRHIRegistry() noexcept = default;

protected:
	std::vector<GPUInfo> graphicsAccelerators;

	IRHIOutputContext* context;
};

