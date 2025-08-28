/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>
#include "Iterator.h"
#include <cstring>



template <typename T>
class Array
{
public:
	Array()
	{
		capacity = RECOMMENDED_CAPACITY_PADDING;
		size = 0;

		container = new T[capacity]();
	}

	~Array()
	{
		if (container)
		{
			delete[] container;
			container = nullptr;

		}
			
		capacity = 0, size = 0;
	}

	Array(const uint32& count)
	{
		capacity = count + RECOMMENDED_CAPACITY_PADDING;
		size = count;

		container = new T[capacity]();
	}

	Array(const Array& other)
	{
		int j = 5;
	};

	Array(T* buffer)
	{
		int j = 5;
	};

	Array& operator=(const Array& other)
	{
		return *this;
	};

	void Add(const T elem, const uint32& at)
	{
		if (size <= at) size = at + 1;

		if (!IsOutOfBounds(at))
		{
			container[at] = elem;
		}
	};

	void Add(const T elem)
	{
		if (size >= capacity)
			Resize(capacity + RECOMMENDED_CAPACITY_PADDING);

		container[size++] = elem;
	};

	void Remove(const uint32& at)
	{
		if (!IsOutOfBounds(at))
		{
			container[at] = {};
		}
	};

	void Pop(const uint32& at)
	{
		if (!IsOutOfBounds(at))
		{
			container[at] = {};
		}
		
		if (at < size)
		{
			memmove(&container[at], &container[at + 1u], (size - at - 1) * sizeof(T));
			size--;
		}
	}

	void Resize(const uint32 Num)
	{		
		if (Num <= capacity) return;

		capacity = Num;
		T* newContainer = new T[capacity]();

		for (uint32 i = 0; i < size; i++)
		{
			newContainer[i] = container[i];
		}

		delete[] container;
		container = newContainer;
	};

	void Reset()
	{
		for (uint32 i = 0; i < capacity; i++)
		{
			container[i] = {};
		}
	};

	T* Data()
	{
		return container;
	}

	uint32 GetSize() const
	{
		return size;
	};

	T& operator[](const uint32& index)
	{
		if (IsOutOfBounds(index))
		{
			return container[0];
		}

		return container[index];
	};

	operator bool()
	{
		return size > 0;
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
