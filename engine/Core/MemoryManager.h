/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>

class MemoryManager
{
public:
	static void Initialize();

	static void Shutdown();

	static void* Allocate();

	static void Deallocate();

	static constexpr uint32 GetSize(void* data);
private:
	uint64 availableMemoryOnTheRig = 0;

	uint64 recommendedByPool = 0;

	void* pool;
};

