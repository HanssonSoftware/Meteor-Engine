/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "MemoryManager.h"
#include <Platform/PlatformLayout.h>
#include <Logging/LogMacros.h>
#include <Layers/SystemLayer.h>

#include <Windows/Windows.h>

LOG_ADDCATEGORY(Arena);

static double engineRecommendedPercent = 0.15;


void MemoryManager::Initialize(const double& RequiredMinimum)
{
	if (!object) object = new MemoryManager();
	engineRecommendedPercent = (RequiredMinimum > 0.0 && RequiredMinimum <= 1.0) ? RequiredMinimum : 0.1;

#ifdef MR_PLATFORM_WINDOWS
	MEMORYSTATUSEX longlong = {};
	longlong.dwLength = sizeof(MEMORYSTATUSEX);

	if (!GlobalMemoryStatusEx(&longlong))
	{
		MR_LOG(LogArena, Fatal, "GlobalMemoryStatusEx failed with: %s", *Layer::GetSystemLayer()->GetError());
	}

	object->totalMemoryOnPC = longlong.ullTotalPhys;

	size_t requiredByPercent = (size_t)(longlong.ullTotalPhys * engineRecommendedPercent);
	if (requiredByPercent < (size_t)object->requiredMinimumInBytes)
	{
		MR_LOG(LogArena, Fatal, "Your PC's memory is too low!");
	}

	object->begin = (char*)VirtualAlloc(nullptr, requiredByPercent, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
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

	object->begin = nullptr;
	object->end = nullptr;

	delete object;
}

void* MemoryManager::Allocate(const size_t& size)
{
	MemoryData data = FindAvailable(size);

	if (data.size == 0)
	{
		MR_LOG(LogArena, Fatal, "Out of memory. Could not allocate %llu bytes.", size);
		return nullptr;
	}

	void* ptr = object->begin + data.offset;
	heap.push_back({ data.offset, size, true });
	return ptr;
}

void MemoryManager::Deallocate(void* data)
{
	if (!data)
		return;

	size_t offset = (char*)data - object->begin;

	for (MemoryData& entry : heap)
	{
		if (entry.offset == offset && entry.used)
		{
			entry.used = false;
			return;
		}
	}

	MR_LOG(LogArena, Warn, "Tried to deallocate unknown or already freed memory at offset %llu", offset);
}

constexpr uint32 MemoryManager::GetSize(void* data)
{
	if (!data) return 0;
	size_t offset = (char*)data - object->begin;

	for (const MemoryData& entry : heap)
	{
		if (entry.offset == offset && entry.used)
			return (uint32)entry.size;
	}

	return 0;
}

MemoryManager::MemoryData MemoryManager::FindAvailable(const size_t& size)
{
	size_t currentOffset = 0;

	for (const MemoryData& entry : heap)
	{
		if (!entry.used && entry.size >= size)
		{
			return { entry.offset, size, true };
		}
		currentOffset = std::max(currentOffset, entry.offset + entry.size);
	}

	if ((object->begin + currentOffset + size) > object->end)
		return { 0, 0, false };

	return { currentOffset, size, true };
}

