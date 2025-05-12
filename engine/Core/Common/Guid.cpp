/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Guid.h"
#include <Logging/Log.h>
#ifdef _WIN64 
#include <Objbase.h>
#pragma comment(lib, "Kernel32.lib")
#pragma warning(disable : 6386)
#endif

Guid::Guid()
{
#ifdef _WIN64
	UUID temp;
	
	if (CoCreateGuid(&temp) != S_OK)
	{
		//THROW_EXCEPTION("Unable To Generate UUID!");
	}

	wchar_t buffer[40];
	if (StringFromGUID2(temp, buffer, sizeof(buffer)) == 0)
	{
		//THROW_EXCEPTION("Unable To Normalise UUID!")
	}

	id = buffer;
#else
	THROW_EXCEPTION("Not Implemented!");
#endif
}

Guid::~Guid()
{
}
