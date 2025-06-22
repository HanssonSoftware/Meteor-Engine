/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "MemoryManager.h"
#include <Platform/PlatformLayout.h>
#include <Logging/LogMacros.h>

#include <Windows/Windows.h>

static constexpr const float engineRecommendedPercent = 0.2f;

static MemoryManager object;

void MemoryManager::Initialize()
{
#ifdef MR_PLATFORM_WINDOWS
	MEMORYSTATUSEX longlong = {};
	longlong.dwLength = sizeof(MEMORYSTATUSEX);

	if (GlobalMemoryStatusEx(&longlong) == 0)
	{
		DWORD a = GetLastError();
		int j = 5;
	}

	object.availableMemoryOnTheRig = longlong.ullTotalPhys;

	uint64 requiredByPercent = longlong.ullTotalPhys * engineRecommendedPercent;

	object.pool = VirtualAlloc(0, requiredByPercent, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
	MR_ASSERT(object.pool != nullptr, "Failed to reserve, the engine recommended memory! Application exiting...");
#endif // MR_PLATFORM_WINDOWS
}

void MemoryManager::Shutdown()
{
	if (VirtualFree(object.pool, 0, MEM_RELEASE) == 0)
	{
		DWORD a = GetLastError();

		int j = 5;
	}
}

void* MemoryManager::Allocate()
{
	void* const place = nullptr;

	return place;
}

void MemoryManager::DeAllocate()
{
}

constexpr uint32 MemoryManager::GetSize(void* data)
{
	return sizeof(data);
}
