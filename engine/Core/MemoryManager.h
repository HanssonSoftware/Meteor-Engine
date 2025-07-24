/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>

struct MemoryManager
{
	static void Initialize(const float RequiredMinimum);

	static void Shutdown();

	static void* Allocate(const size_t& size);

	static void Deallocate();

	static constexpr uint32 GetSize(void* data);
private:

	uint64 availableMemoryOnTheRig = 0;

	uint64 recommendedByPool = 0;

	struct 
	{
		void* begin;

		void* end;

		size_t offset;
	};
};

