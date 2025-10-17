/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "MemoryManager.h"
#include <Platform/PlatformLayout.h>

#include <Layers/SystemLayer.h>

#include <Platform/Platform.h>


LOG_ADDCATEGORY(Arena);

static double engineRecommendedPercent = 0.15;

MemoryManager& MemoryManager::Get()
{
	static MemoryManager instance;
	return instance;
}

void MemoryManager::Initialize(const double& RequiredMinimum)
{
	if (!object) object = new MemoryManager();
	engineRecommendedPercent = (RequiredMinimum > 0.0 && RequiredMinimum <= 1.0) ? RequiredMinimum : 0.1;

#ifdef MR_PLATFORM_WINDOWS
	MEMORYSTATUSEX longlong = {};
	longlong.dwLength = sizeof(MEMORYSTATUSEX);

	if (!GlobalMemoryStatusEx(&longlong))
	{
		MR_LOG(LogArena, Fatal, "GlobalMemoryStatusEx failed with: %s", *Platform::GetError());
	}

	object->totalMemoryOnPC = longlong.ullTotalPhys;

	uint64_t requiredByPercent = (uint64_t)(longlong.ullTotalPhys * engineRecommendedPercent);
	if (requiredByPercent < (uint64_t)object->requiredMinimumInBytes)
	{
		MR_LOG(LogArena, Fatal, "Your PC's memory is too low! Consider buying more RAM.");
	}

	object->begin = (char*)VirtualAlloc(nullptr, requiredByPercent, MEM_RESERVE, PAGE_READWRITE);
	
	MR_ASSERT(object->begin != nullptr, "Failed to reserve, the engine recommended memory! Application exiting...");
#endif // MR_PLATFORM_WINDOWS

	object->end = (char*)object->begin + requiredByPercent;
}

void MemoryManager::Shutdown()
{
	if (!VirtualFree(object->begin, 0, MEM_RELEASE | MEM_DECOMMIT))
	{
		MR_LOG(LogArena, Fatal, "VirtualFree failed with: %s", *Platform::GetError());
	}

	object->begin = nullptr;
	object->end = nullptr;

	delete object;
}