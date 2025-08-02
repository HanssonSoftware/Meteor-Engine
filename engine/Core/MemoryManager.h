/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>
#include <basetsd.h>
#include <vector>

struct MemoryManager
{
	static void Initialize(const float RequiredMinimum);

	static void Shutdown();

	static void* Allocate(const size_t& size);

	static void Deallocate(void* data);

	static constexpr uint32 GetSize(void* data);

	static constexpr void SetMinimumSize(const size_t& requiredMinimumInBytes) noexcept
	{
		if (!object)
			object = new MemoryManager();

		object->requiredMinimumInBytes = requiredMinimumInBytes;
	};

protected:
	MemoryManager() = default;

	virtual ~MemoryManager() noexcept = default;

	struct MemoryData
	{
		size_t offset = 0;

		size_t size = 0;

		bool used = false;
	};

	static MemoryData FindAvailable(const size_t& size);

	size_t requiredMinimumInBytes = 1'000'000'000; // 1GB ~= 1000 MB

	size_t totalMemoryOnPC = 0;

	char* begin = nullptr;

	char* end = nullptr;

	static inline std::vector<MemoryData> heap;

	static inline MemoryManager* object;
};

