/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#include "MemoryManager.h"
#include <Log/LogMacros.h>

STATIC_LOG_ADDCATEGORY(Memory);

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
	MR_ASSERT(occupiedLocation != nullptr, TEXT("Unable to allocate memory!"));

	MemoryManager::Get().usedMemoryInBytes += size;
	MemoryManager::Get().usedTotalUsedMemoryInBytes += size;

	return occupiedLocation;
}

void mrfree(void* ptr)
{
	MemoryManager::Get().usedMemoryInBytes -= sizeof(ptr);
	return free(ptr);
}

void* operator new(size_t size)
{
	void* allocated = malloc(size);
	if (!allocated) {
		MR_LOG(LogMemory, Fatal, TEXT("Unable to allocate memory with size: %d"), size);
	}

	MemoryManager::Get().usedMemoryInBytes += size;
	MemoryManager::Get().usedTotalUsedMemoryInBytes += size;
	return allocated;
}

void operator delete(void* location, size_t size) noexcept
{
	MemoryManager::Get().usedMemoryInBytes -= size;
	return free(location);
}