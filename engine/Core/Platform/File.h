/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "FileHelpers.h"

#include <Types/String.h>
#include <Logging/LogMacros.h>
#include <Common/Pointers.h>

LOG_ADDCATEGORY(FileSystem);

class IFile
{
public:
	IFile() = default;

	virtual ~IFile();

	virtual void* GetFileHandle() = 0;

	virtual void Write(const String buffer) const = 0;

	virtual void Read() = 0;

	virtual void Close() = 0;

	virtual bool ValidDirectory(const String& directory, bool bCreateIfNotExist = false) = 0;

	String GetName() const { return fileName; };

	virtual void SetName(String content) {};
	 
	uint32 GetSize() const { return size; }

	bool IsValid() const
	{
		return bWasInitSucceded;
	}
		
	/** Returns true, if the file was "created" from the hard disk. */
	bool IsPersistent() const
	{
		return !bWasCreatedByProgramatically;
	}

	void Delete();

	const char* GetBuffer() const noexcept
	{
		return buffer;
	}

	operator char* ()
	{
		return buffer;
	}

	operator const char* ()
	{
		return buffer;
	}

protected:
	bool bWasCreatedByProgramatically = false;

	bool bWasInitSucceded = false;

	uint32 size = 0;

	char* buffer;

	String fileName;
};
