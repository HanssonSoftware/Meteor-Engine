/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "MemoryManager.h"
#include <Platform/PlatformLayout.h>
#include <Logging/LogMacros.h>
#include <Layers/SystemLayer.h>

#include <Windows/Windows.h>

LOG_ADDCATEGORY(Arena);

static float engineRecommendedPercent = 0.15f;


void MemoryManager::Initialize(const float RequiredMinimum)
{
	if (!object) object = new MemoryManager();
	engineRecommendedPercent = RequiredMinimum > 0.f ? RequiredMinimum : 0.1f;

#ifdef MR_PLATFORM_WINDOWS
	MEMORYSTATUSEX longlong = {};
	longlong.dwLength = sizeof(MEMORYSTATUSEX);

	if (!GlobalMemoryStatusEx(&longlong))
	{
		MR_LOG(LogArena, Fatal, "GlobalMemoryStatusEx failed with: %s", *Layer::GetSystemLayer()->GetError());
		return;
	}

	object->totalMemoryOnPC = longlong.ullTotalPhys;

	size_t requiredByPercent = (size_t)(longlong.ullTotalPhys * engineRecommendedPercent);
	if (requiredByPercent < (size_t)object->requiredMinimumInBytes)
	{
		MR_LOG(LogArena, Fatal, "Your PC's memory is too low!");
		return;
	}

	object->begin = VirtualAlloc(nullptr, requiredByPercent, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
	MR_ASSERT(object->begin != nullptr, "Failed to reserve, the engine recommended memory! Application exiting...");
#endif // MR_PLATFORM_WINDOWS

	object->end = (char*)object->begin + requiredByPercent;
}

void MemoryManager::Shutdown()
{
	if (!VirtualFree(object->begin, 0, MEM_RELEASE))
	{
		MR_LOG(LogArena, Fatal, "VirtualFree failed with: %s", *Layer::GetSystemLayer()->GetError());
	}

	delete object;
}

void* MemoryManager::Allocate(const size_t& size)
{
	void* place = (char*)object->begin + object->offset;



	return place;
}

void MemoryManager::Deallocate()
{
}

constexpr uint32 MemoryManager::GetSize(void* data)
{
	return sizeof(data);
}
