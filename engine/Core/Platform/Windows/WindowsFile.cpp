/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

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


bool WindowsFile::ValidDirectory(const String& directory, bool bCreateIfNotExist)
{
	if (directory.IsEmpty())
	{
		MR_LOG(LogFileSystem, Error, "Directory cannot be empty!");
		return false;
	}

	while (true)
	{

	}

	return false;
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

void WindowsFile::Write(const String buffer) const
{
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		MR_LOG(LogFileSystem, Error, "File handle is invalid!");
		return;
	}

	DWORD written = 0;
	if (!WriteFile(fileHandle, buffer.Chr(), (DWORD)buffer.Length(), &written, nullptr))
	{
		if (SystemLayer* Layer = Layer::GetSystemLayer())
		{
			const String a = Layer->GetError();
			MR_LOG(LogFileSystem, Error, "WriteFile returned: %s", a.Chr());
			return;
		}

		MR_LOG(LogFileSystem, Error, "WriteFile error!");
	}

	FlushFileBuffers(fileHandle);
}

void WindowsFile::Read()
{
	LARGE_INTEGER lg;
	if (GetFileSizeEx(fileHandle, &lg) == 0)
	{
		if (SystemLayer* Layer = Layer::GetSystemLayer())
		{
			const String a = Layer->GetError();
			MR_LOG(LogFileSystem, Error, "GetFileSizeEx returned: %s", a.Chr());		
			return;
		}

		MR_LOG(LogFileSystem, Error, "GetFileSizeEx error!");
		return;
	}

	DWORD write = 0;

	if (lg.QuadPart > 0)
		buffer = new char[lg.QuadPart + 1];
	else
		buffer = new char[1];


	if (buffer)
	{
		if (!ReadFile(fileHandle, buffer, (DWORD)lg.QuadPart, &write, 0))
		{
			if (SystemLayer* Layer = Layer::GetSystemLayer())
			{
				const String a = Layer->GetError();
				MR_LOG(LogFileSystem, Error, "ReadFile returned: %s", a.Chr());
				return;
			}

			MR_LOG(LogFileSystem, Error, "ReadFile error!");
			return;
		}

		size = (uint32)lg.QuadPart;
		buffer[write] = '\0';

		//const int requiredAmount = MultiByteToWideChar(CP_UTF8, 0, narrowBuffer, write, 0, 0);
		//buffer = new char[requiredAmount + 1];

		//if (requiredAmount > 0)
		//{
		//	const int writtenAmount = /*MultiByteToWideChar(CP_UTF8, 0, narrowBuffer, write, buffer, requiredAmount)*/4;
		//	if (writtenAmount == 0 || writtenAmount != requiredAmount)
		//	{
		//		if (SystemLayer* Layer = Layer::GetSystemLayer())
		//		{
		//			const String a = Layer->GetError();
		//			MR_LOG(LogFileSystem, Error, TEXT("MultiByteToWideChar returned: %s"), a.Chr());				
		//			return;
		//		}
		//	}
		//}

		//buffer[write] = L'\0';
		//delete[] narrowBuffer;
	}
}
