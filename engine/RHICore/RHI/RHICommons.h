/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Vector.h>
#include <vector>

enum SystemLayout
{
	SYSTEM_LAYOUT_WINDOWS = 0,
	SYSTEM_LAYOUT_LINUX = 1,
	SYSTEM_LAYOUT_MACOS = 2,
	SYSTEM_LAYOUT_ANDROID = 3,
	SYSTEM_LAYOUT_IOS = 4
};

struct MonitorInfo
{
	MonitorInfo(const String name, const Vector2<uint32> size, const int index)
		: name(name)
		, size(size)
		, index(index)
	{

	}

	String name;

	Vector2<uint32> size = { 0, 0 };

	int index = -1;

	inline bool GetIsPrimary() const noexcept { return index == 0; }
};

struct GPUInfo
{
	GPUInfo(const String name, const int index, const int sizeLimit)
		: name(name)
		, index(index)
		, videoMemorySizeInMB(sizeLimit)
	{

	}

	GPUInfo() = delete;

	String name;

	int index = -1;

	int videoMemorySizeInMB = 0;

	std::vector<MonitorInfo> outputs;
};

enum RHIHealthStatus
{
	RHI_HEALTH_STATUS_NONE = 0,
	RHI_HEALTH_STATUS_INITIALIZE = 1,
	RHI_HEALTH_STATUS_RUNNING = 2,
	RHI_HEALTH_STATUS_SHUT_DOWN = 3,
	RHI_HEALTH_STATUS_RECOVERABLE = 4,
	RHI_HEALTH_STATUS_MELTDOWN = 5
};