/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "WindowsFile.h"
#include <Windows/Windows.h>

#include <handleapi.h>
#include <PathCch.h>
#include <Shlwapi.h>

#include <Application.h>
#include <Layers/SystemLayer.h>

#pragma comment(lib, "Pathcch.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Kernel32.lib")

WindowsFile::~WindowsFile()
{
	if (fileHandle)
	{
		FlushFileBuffers(fileHandle);
		CloseHandle(fileHandle);
	}
}

void* WindowsFile::GetFileHandle()
{
    return fileHandle;
}

void WindowsFile::Close()
{
	if (fileHandle)
	{
		if (buffer)
		{
			delete[] buffer;
			buffer = nullptr;
		}

		FlushFileBuffers(fileHandle);
		CloseHandle(fileHandle);
		fileHandle = nullptr;
	}

	delete this;
}

void WindowsFile::Delete()
{
	if (!::DeleteFileW(nullptr))
	{
		MR_LOG(LogFileSystem, Error, "Unable to delete file! %s", *Layer::GetSystemLayer()->GetError());
	}
}

void WindowsFile::Write(const String& buffer) const
{
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		MR_LOG(LogFileSystem, Error, "File handle is invalid!");
		return;
	}

	DWORD written = 0;
	if (!WriteFile(fileHandle, buffer.Chr(), (DWORD)buffer.Length(), &written, nullptr))
	{
		MR_LOG(LogFileSystem, Error, "WriteFile error! %s", *Layer::GetSystemLayer()->GetError());
		return;
	}

	FlushFileBuffers(fileHandle);
}

void WindowsFile::Read()
{
	if (fileHandle == INVALID_HANDLE_VALUE) 
	{
		MR_LOG(LogFileSystem, Error, "File handle is invalid!");
		return;
	}


	LARGE_INTEGER lg;
	if (!GetFileSizeEx(fileHandle, &lg))
	{
		MR_LOG(LogFileSystem, Error, "GetFileSizeEx returned: %s", *Layer::GetSystemLayer()->GetError());
		return;		
	}

	buffer = new char[lg.QuadPart > 0 ? lg.QuadPart + 1 : 1];

	if (buffer)
	{
		DWORD write = 0;

		if (!ReadFile(fileHandle, buffer, (DWORD)lg.QuadPart, &write, 0))
		{
			MR_LOG(LogFileSystem, Error, "ReadFile returned: %s", *Layer::GetSystemLayer()->GetError());
			return;
		}

		size = (uint32)lg.QuadPart;
		buffer[write] = '\0';
	}
}
