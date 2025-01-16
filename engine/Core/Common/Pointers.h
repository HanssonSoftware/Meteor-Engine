/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Common/MemoryManager.h>

template<typename T>
class ScopedPtr
{
public:
	ScopedPtr();

	ScopedPtr(T* newPtr);
	
	~ScopedPtr();

	T* Get()
	{
		return *(this->ptr);
	}

	T* operator->()
	{
		return ptr;
	}

	void operator=(const ScopedPtr<T>& other) noexcept
	{
		if (this->ptr == &other.ptr)
			return;

		if (ptr == nullptr)
		{
			ptr = (T*)mrmalloc(sizeof(T));
		}

		wmemmove(this->ptr, other.ptr, sizeof(other.ptr));
		if (other.ptr != nullptr)
		{
			mrfree(other.ptr);
		}
	}

private:
	T* ptr = nullptr;
};

template<typename T>
inline ScopedPtr<T>::ScopedPtr()
{
	ptr = nullptr;
}

template<typename T>
inline ScopedPtr<T>::ScopedPtr(T* newPtr)
	: ptr(newPtr)
{
	
}

template<typename T>
inline ScopedPtr<T>::~ScopedPtr()
{
	int i = 23;
	if (ptr != nullptr)
	{
		mrfree(ptr);
	}
}
