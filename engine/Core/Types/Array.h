/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>
#include <cstring>

template <class T>
class Array
{
public:
	Array();

	~Array();

	void Resize(const uint32 Num);

	void Reset();

	uint32 GetSize() const
	{
		return size;
	}

	T& operator[](int index)
	{
		if (index > (int)size || index < 0) return container[0];

		return container[index];
	}

private:
	uint32 capacity;

	uint32 size;

	T* container;
};


template<class T>
inline Array<T>::Array()
{
	capacity = 2;
	size = capacity - 1;

	container = new T[capacity]();
}

template<class T>
inline Array<T>::~Array()
{
	if (container && size > 0)
	{
		delete[] container;

		capacity, size = 0;
	}
}

template<class T>
void Array<T>::Reset()
{
	for (uint32 i = 0; i < capacity; i++)
	{
		container[i] = {};
	}
}

template<class T>
void Array<T>::Resize(const uint32 Num)
{
	if (Num == size)
		return;

	size = Num;

	T* newContainer = new T[size]();

	memmove(newContainer, container, size);
	delete[] container;

	container = newContainer;
}