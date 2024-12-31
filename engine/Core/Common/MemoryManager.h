/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Log/Log.h>

/**
 * Memory Management system. Overrides some of the memory related 
 * functions/operators.
 */
class MemoryManager
{
	friend class Application;
public:
	static MemoryManager& Get();

	~MemoryManager();

	size_t getTotalUsedMemory() const;

	size_t getUsedMemory() const;

	size_t usedMemoryInBytes = 0;

	size_t usedTotalUsedMemoryInBytes = 0;
private:
	bool bQuickMemoryLogging = false;
};

extern void* mrmalloc(size_t size);

extern void mrmove(const void* source, void* target);

extern void mrfree(void* ptr);

extern void* operator new(size_t size);

extern void operator delete(void* location, size_t size) noexcept;

extern void operator delete[](void* location, size_t size) noexcept;