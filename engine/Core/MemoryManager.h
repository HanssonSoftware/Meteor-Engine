/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once

#include <basetsd.h>
#include <vector>
#include <Logging/Log.h>

//
#include <Windows/Windows.h>

//	OC = Occupied // AV = Available
//	----------------------------------------------
//	|	OC	| AV |	OC	|						 |
//	----------------------------------------------


struct MemoryManager
{
	static MemoryManager& Get();

	void Initialize(const double& RequiredMinimum);

	void Shutdown();

	template<typename T>
	T* Allocate(const uint64_t& size)
	{
#ifdef MR_PLATFORM_WINDOWS
		void* raw = VirtualAlloc(object->begin + object->currentOffset, size, MEM_COMMIT, PAGE_READWRITE);
#endif // MR_PLATFORM_WINDOWS
		MR_ASSERT((object->begin + object->currentOffset) < object->end, "Page out of range!");

		if (!raw)
		{
			//MR_LOG(LogArena, Fatal, "Out of memory. Could not allocate %llu bytes.", size);
			return nullptr;
		}

		T* ptr = new(raw) T();
		object->currentOffset += size;
		return ptr;
	};

	template<typename T>
	void Deallocate(T* data)
	{
		if (!data)
			return;

		data->~T();
		if (!VirtualFree(data, sizeof(T), MEM_DECOMMIT))
		{
			//MR_LOG(LogArena, Fatal, "%s", *Platform::GetError());
		}

		memset(data, 0xCD, sizeof(T));
		object->currentOffset -= sizeof(T);
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

	uint64_t totalMemoryOnPC = 0;

	uint64_t currentOffset = 0;

	char* begin = nullptr;

	char* end = nullptr;

	static inline std::vector<MemoryData> heap;

	MemoryManager* object;
};
