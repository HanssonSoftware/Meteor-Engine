/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "WindowsFile.h"

#include <Application.h>
#include <Platform/Platform.h>

#include <handleapi.h>
#include <PathCch.h>
#include <Shlwapi.h>

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
			MemoryManager::Get().Deallocate(buffer);
		}

		FlushFileBuffers(fileHandle);
		CloseHandle(fileHandle);
		fileHandle = nullptr;
	}

	MemoryManager::Get().Deallocate(this);
}

void WindowsFile::Delete()
{
	ScopedPtr<wchar_t> fullName = Platform::ConvertToWide(fullPath);
	
	if (!::DeleteFileW(fullName.Get()))
	{
		MR_LOG(LogFileSystem, Error, "Unable to delete file! %s", *Platform::GetError());
	}
}

void WindowsFile::Write(const String* buffer) const
{
	if (buffer != nullptr)
	{
		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			DWORD written = 0;
			if (!WriteFile(fileHandle, buffer->Chr(), (DWORD)buffer->Length(), &written, nullptr))
			{
				MR_LOG(LogFileSystem, Error, "WriteFile error! %s", *Platform::GetError());
				return;
			}

			FlushFileBuffers(fileHandle);
		}
		else
		{
			MR_LOG(LogFileSystem, Error, "File handle is invalid!");
			return;
		}
	}
}

void WindowsFile::Read()
{
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER lg;
		if (!GetFileSizeEx(fileHandle, &lg))
		{
			MR_LOG(LogFileSystem, Error, "GetFileSizeEx returned: %s", *Platform::GetError());
			return;
		}

		buffer = new char[lg.QuadPart > 0 ? lg.QuadPart + 1 : 1]();

		if (buffer)
		{
			DWORD write = 0;

			if (!ReadFile(fileHandle, buffer, (DWORD)lg.QuadPart, &write, nullptr))
			{
				MR_LOG(LogFileSystem, Error, "ReadFile returned: %s", *Platform::GetError());
				return;
			}

			size = (uint32_t)lg.QuadPart;
			buffer[write] = '\0';
		}
	}
	else
	{
		MR_LOG(LogFileSystem, Error, "Invalid file handle!");
		return;
	}
}
