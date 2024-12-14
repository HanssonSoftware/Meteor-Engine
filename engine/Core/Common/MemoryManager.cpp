/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */
#include "MemoryManager.h"
#include <Log/LogMacros.h>

LOG_ADDCATEGORY(Memory);

MemoryManager& MemoryManager::Get()
{
	static MemoryManager instance;
	return instance;
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
		MR_LOG(LogMemory, Error, TEXT("Unable to allocate memory!"));
		return nullptr;
	}

	MemoryManager::Get().usedMemoryInBytes += size;
	MemoryManager::Get().usedTotalUsedMemoryInBytes += size;

	return occupiedLocation;
}

void mrfree(void* ptr)
{
	if (ptr == nullptr)
	{
		MR_LOG(LogMemory, Warn, TEXT("Unable to free memory. Perhaps it is already freed up?"));
		return;
	}

	MemoryManager::Get().usedMemoryInBytes -= sizeof(ptr);
	return free(ptr);
}

void* operator new(size_t size)
{
	void* occupiedLocation = malloc(size);
	if (occupiedLocation == nullptr)
	{
		MR_LOG(LogMemory, Error, TEXT("Unable to allocate memory!"));
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
		MR_LOG(LogMemory, Error, TEXT("Unable to free memory. Perhaps it is already freed up?"));
		return;
	}


	MemoryManager::Get().usedMemoryInBytes -= size;
	return free(location);
}

void operator delete[](void* location, size_t size) noexcept
{
	if (location == nullptr)
	{
		MR_LOG(LogMemory, Error, TEXT("Unable to free memory. Perhaps it is already freed up?"));
		return;
	}


	MemoryManager::Get().usedMemoryInBytes -= size;
	return free(location);
}
