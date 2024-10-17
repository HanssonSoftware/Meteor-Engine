/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#include <Types/String.h>


class File
{
public:
	File(const File&) = delete;

	File() = delete;

	~File();

	File(const String& Name);

protected:
	uint32 size = 0;

	void* fileHandle;

	wchar_t* buffer;
};