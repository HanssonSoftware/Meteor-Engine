/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>

struct MemoryManager
{
	static void Initialize(const float RequiredMinimum);

	static void Shutdown();

	static void* Allocate(const size_t& size);

	static void Deallocate();

	static constexpr uint32 GetSize(void* data);

	static constexpr void SetMinimumSize(const size_t& requiredMinimumInBytes) noexcept
	{
		if (!object)
			object = new MemoryManager();

		object->requiredMinimumInBytes = requiredMinimumInBytes;
	};

private:
	MemoryManager() = default;

	virtual ~MemoryManager() noexcept = default;

	static inline MemoryManager* object;

	size_t requiredMinimumInBytes = 1'000'000'000;

	size_t totalMemoryOnPC = 0;

	struct 
	{
		void* begin = nullptr;

		void* end = nullptr;

		size_t offset = 0;
	};
};

