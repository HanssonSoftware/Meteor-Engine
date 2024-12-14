/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Log/LogMacros.h>

LOG_ADDCATEGORY(FileSystem);

struct FileOpenRules
{
	int shareMode = -1;

};

class File
{
public:
	File(const File&) = delete;

	File() = delete;

	~File();

	File(const String Name);

	void Write(const wchar_t* buffer);

	void Read();

	String getName() const { return Name; };

	void setName(const String content);
protected:

	uint32 size = 0;

	void* fileHandle;

	wchar_t* buffer;

private:
	String Name;
};