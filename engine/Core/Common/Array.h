/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Types/PlatformDefs.h>

template <class T>
class Array
{
public:
	Array() = default;

	Array(T* p)
		: p(p)
	{
	}

	void Resize(uint32 Num);

	bool operator==(const Array& other) const
	{
		return p == other.p;
	};

private:
	T* p;
};

