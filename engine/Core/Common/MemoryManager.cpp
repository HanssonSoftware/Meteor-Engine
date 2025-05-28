/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "MemoryManager.h"
#include <Logging/LogMacros.h>
#include <Core/Application.h>

LOG_ADDCATEGORY(Memory);

MemoryManager& MemoryManager::Get()
{
	static MemoryManager instance;
	return instance;
}

MemoryManager::~MemoryManager()
{
	if (bQuickMemoryLogging)
	{
		MR_LOG(LogMemory, Log, "Total used memory:\t%ld\t\tCurrently used memory (not freed up):\t%ld", usedTotalUsedMemoryInBytes, usedMemoryInBytes);
	}
}

size_t MemoryManager::getTotalUsedMemory() const
{
	return usedTotalUsedMemoryInBytes;
}

size_t MemoryManager::getUsedMemory() const
{
	return usedMemoryInBytes;
}

void* mrmalloc(size_t size)
{
	void* occupiedLocation = malloc(size);
	if (occupiedLocation == nullptr)
	{
		MR_LOG(LogMemory, Error, "Unable to allocate memory!");
		return nullptr;
	}

	MemoryManager::Get().usedMemoryInBytes += size;
	MemoryManager::Get().usedTotalUsedMemoryInBytes += size;

	return occupiedLocation;
}

void mrmove(const void* source, void* target)
{
}

void mrfree(void* ptr)
{
	if (ptr == nullptr)
	{
		MR_LOG(LogMemory, Warn, "Unable to free memory. Perhaps it is already freed up?");
		return;
	}

	MemoryManager::Get().usedMemoryInBytes -= sizeof(ptr);
	free(ptr);
	ptr = nullptr;
}

void* operator new(size_t size)
{
	void* occupiedLocation = malloc(size);
	if (occupiedLocation == NULL)
	{
		MR_LOG(LogMemory, Error, "Unable to allocate memory!");
		return nullptr;
	}

	MemoryManager::Get().usedMemoryInBytes += size;
	MemoryManager::Get().usedTotalUsedMemoryInBytes += size;
	return occupiedLocation;
}

void operator delete(void* location, size_t size) noexcept
{
	if (location == nullptr)
	{
		MR_LOG(LogMemory, Error, "Unable to free memory. Perhaps it is already freed up?");
		return;
	}


	MemoryManager::Get().usedMemoryInBytes -= size;
	return free(location);
}

void operator delete[](void* location, size_t size) noexcept
{
	if (location == nullptr)
	{
		MR_LOG(LogMemory, Error, "Unable to free memory. Perhaps it is already freed up?");
		return;
	}


	MemoryManager::Get().usedMemoryInBytes -= size;
	return free(location);
}
