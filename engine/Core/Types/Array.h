/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>
#include <Logging/LogMacros.h>
#include "Iterator.h"
#include <cstring>

LOG_ADDCATEGORY(Array);

template <class T>
class Array : public Iterator<T>
{
public:
	Array()
	{
		capacity = RECOMMENDED_CAPACITY_PADDING;
		size = 1;

		container = new T[capacity]();
	}

	~Array()
	{
		if (container && size > 0)
		{
			delete[] container;
			container = nullptr;

			capacity = 0, size = 0;
		}
	}

	Array(const uint32& count)
	{
		capacity = count + RECOMMENDED_CAPACITY_PADDING;
		size = count;

		container = new T[capacity]();
	}

	Array(const Array& other) = delete;

	Array& operator=(const Array& other) = delete;

	void Add(const T* elem, const uint32& at)
	{
		size = size < at ? at : size;

		if (!IsOutOfBounds(at))
		{
			container[at] = *elem;
		}
	}

	void Add(const T* elem)
	{
		container[size] = *elem;
		size++;

		if (size >= capacity)
			Resize(capacity + RECOMMENDED_CAPACITY_PADDING);

	}

	void Remove(const uint32& at)
	{
		if (!IsOutOfBounds(at))
		{
			container[at] = {};
		}
	}

	void Pop(const uint32& at)
	{
		if (!IsOutOfBounds(at))
		{
			container[at] = {};
		}
		
		if (at < size)
		{
			memmove(&container[at], &container[at + 1u], (size - at) * sizeof(container[0]));
		}
	}

	void Resize(const uint32 Num)
	{		
		if (Num == size) return;

		size = Num;
		capacity = Num + RECOMMENDED_CAPACITY_PADDING;

		T* newContainer = new T[capacity]();

		memcpy(newContainer, container, size);
		delete[] container;

		container = newContainer;
	};

	void Reset()
	{
		for (uint32 i = 0; i < capacity; i++)
		{
			container[i] = {};
		}
	}

	uint32 GetSize() const
	{
		return size;
	}

	T& operator[](const uint32& index)
	{
		if (IsOutOfBounds(index))
		{
			MR_LOG(LogArray, Error, "Trying to reach out of bounds value: %d\tOf: %u", index, size);
			return container[0];
		}

		return container[index];
	}

	bool IsOutOfBounds(const uint32& index) { return index >= capacity; };

	Iterator<T> begin() { return Iterator(&container[0]); }

	Iterator<T> end() { return Iterator(&container[size]); }
private:

	static constexpr const uint32 RECOMMENDED_CAPACITY_PADDING = 4;

	uint32 capacity = 0;

	uint32 size = 0;

	T* container = nullptr;
};
