/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#ifdef _WIN32 
#include <Objbase.h>
#pragma comment(lib, "Kernel32.lib")
#endif

class Guid
{
public:
	Guid();

	~Guid();

	GUID getID() const
	{
		return id;
	}

private:
	GUID id;
};

