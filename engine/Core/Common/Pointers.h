/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Common/MemoryManager.h>

/**
 * ScopedPtr owns a pointer, so you don’t have to think about deleting it.
 * When this thing goes out of scope, the pointer is gone too.
 * No magic, no bullshit.
 */
template<typename T>
class ScopedPtr
{
public:
	ScopedPtr();

	explicit ScopedPtr(T* newPtr) 
		: ptr(newPtr) 
	{
	
	}
	
	~ScopedPtr();

	T* Get() const
	{
		return this->ptr;
	}

	T* operator->() const
	{
		return this->ptr;
	}

	ScopedPtr(const ScopedPtr&) = delete;

	ScopedPtr& operator=(const ScopedPtr&) = delete;

	ScopedPtr(ScopedPtr&& other) noexcept
	{
		ptr = other.ptr;
		other.ptr = nullptr;
	}

	ScopedPtr& operator=(ScopedPtr&& other) noexcept
	{
		if (this != &other)
		{
			if (ptr)
				mrfree(ptr);

			ptr = other.ptr;
			other.ptr = nullptr;
		}
		return *this;
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
inline ScopedPtr<T>::~ScopedPtr()
{
	if (ptr != nullptr)
	{
		mrfree(ptr);
		ptr = nullptr;
	}
}
