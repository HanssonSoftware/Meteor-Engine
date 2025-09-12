/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once

#include <basetsd.h>
#include <vector>


struct MemoryManager
{
	static void Initialize(const double& RequiredMinimum);

	static void Shutdown();

	static void* Allocate(const uint32_t& size);

	static void Deallocate(void* data);

	static constexpr uint32_t GetSize(void* data);

	static constexpr void SetMinimumSize(const uint32_t& requiredMinimumInBytes) noexcept
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
		uint32_t offset = 0;

		uint32_t size = 0;

		bool used = false;
	};

	static MemoryData FindAvailable(const uint32_t& size);

	uint32_t requiredMinimumInBytes = 1'000'000'000; // 1GB ~= 1000 MB

	uint32_t totalMemoryOnPC = 0;

	char* begin = nullptr;

	char* end = nullptr;

	static inline std::vector<MemoryData> heap;

	static inline MemoryManager* object;
};

