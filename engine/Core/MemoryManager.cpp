/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "MemoryManager.h"
#include <Platform/PlatformLayout.h>
#include <Logging/LogMacros.h>
#include <Layers/SystemLayer.h>

#include <Windows/Windows.h>

LOG_ADDCATEGORY(Arena);

static constexpr const float engineRecommendedPercent = 0.15f;

static MemoryManager object;

void MemoryManager::Initialize()
{
#ifdef MR_PLATFORM_WINDOWS
	MEMORYSTATUSEX longlong = {};
	longlong.dwLength = sizeof(MEMORYSTATUSEX);

	if (!GlobalMemoryStatusEx(&longlong))
	{
		MR_LOG(LogArena, Fatal, "GlobalMemoryStatusEx failed with: %s", *Layer::GetSystemLayer()->GetError());
		return;
	}

	object.availableMemoryOnTheRig = longlong.ullTotalPhys;

	uint64 requiredByPercent = longlong.ullTotalPhys * engineRecommendedPercent;

	object.pool = VirtualAlloc(nullptr, requiredByPercent, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
	MR_ASSERT(object.pool != nullptr, "Failed to reserve, the engine recommended memory! Application exiting...");
#endif // MR_PLATFORM_WINDOWS
}

void MemoryManager::Shutdown()
{
	if (!VirtualFree(object.pool, 0, MEM_RELEASE))
	{
		MR_LOG(LogArena, Fatal, "VirtualFree failed with: %s", *Layer::GetSystemLayer()->GetError());
		return;
	}
}

void* MemoryManager::Allocate()
{
	void* const place = nullptr;

	return place;
}

void MemoryManager::Deallocate()
{
}

constexpr uint32 MemoryManager::GetSize(void* data)
{
	return sizeof(data);
}
