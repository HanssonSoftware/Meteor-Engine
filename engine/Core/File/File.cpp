/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "File.h"
#include <string>
#include <Platform/Microsoft/MinWin.h>
#include <Log/Exception.h>
#include <Common/MemoryManager.h>

File::~File()
{
#ifdef _WIN32

	if (fileHandle)
	{
		CloseHandle(fileHandle);
	}

#endif // _WIN32

	if (buffer != nullptr)
	{
		mrfree(buffer);
	}
}

File::File(const String Name/*, int readType, int shareMode, EOpenMode openMode*/)
{
	fileHandle = nullptr;
	if (Name.isEmpty())
	{
		THROW_EXCEPTION("How is the file supposed to be named?");
	}

	//buffer = nullptr;

#ifdef _WIN32
	fileHandle = CreateFile(
		Name.Chr(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 
		nullptr
	);

	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		Logger::Get().dispatchLastError();
		return;
	}


#endif // _WIN32
}

void File::Write(const wchar_t* buffer)
{
	if (!fileHandle)
	{
		MR_LOG(LogFileSystem, Error, TEXT("Invalid Handle!"));
		return;
	}

	if (!buffer)
	{
		MR_LOG(LogFileSystem, Error,TEXT("Invalid Buffer to Write!"));
		return;
	}

#ifdef _WIN32
	const size_t count = wcslen(buffer);

	if (WriteFile((HANDLE)fileHandle, buffer, (DWORD)count, 0, 0))
	{
		FlushFileBuffers(fileHandle);
	}
	else
	{
		MR_LOG(LogFileSystem, Error, TEXT("Failed Writing to File! %s"), getName().Chr());
	}

#endif // _WIN32
}

void File::Read()
{
	if (!fileHandle)
	{
		MR_LOG(LogFileSystem, Error, TEXT("Unable to Read to Buffer!"));
		return;
	}

#ifdef _WIN32
	LARGE_INTEGER lg;
	GetFileSizeEx(fileHandle, &lg);
	DWORD write = 1;

	buffer = (wchar_t*)mrmalloc((lg.HighPart + 1) * sizeof(wchar_t));

	if (!ReadFile(fileHandle, buffer, lg.LowPart, &write, 0))
	{
		
	}


	Logger::Get().dispatchLastError();
#endif
	String superBuffer(buffer);

}

void File::setName(const String content)
{
	if (content.isEmpty())
	{
		THROW_EXCEPTION("How is the file supposed to be named?");
	}

	if (Name == content)
	{
		MR_LOG(LogFileSystem, Error, TEXT("Renaming as the same as before? This won't happen."));
		return;
	}

#ifdef _WIN32
	if (MoveFile(Name.Chr(), content.Chr()))
	{
		Name = content;
	}
#endif // _WIN32
}