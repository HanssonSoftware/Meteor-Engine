/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WindowsFile.h"
#include <Windows/Windows.h>
#include <handleapi.h>
#include <PathCch.h>
#include <Shlwapi.h>
#include <Application.h>
#include <Layers/OSLayer.h>

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

void* WindowsFile::GetFileHandle() const
{
    return fileHandle;
}

FileStatus WindowsFile::Open(const String Name, int openRules, FileOverrideRules overrideRules)
{
	IFile::Open(Name, openRules, overrideRules);

	OSLayer* systemLayer = Layer::GetSystemLayer();

	MR_ASSERT(systemLayer != nullptr, "System layer is not initialized!");

	const wchar_t* data = systemLayer->ConvertToWide(Name.Chr());

	wchar_t lookUp[MAX_PATH];
	wcscpy_s(lookUp, data);
	PathCchRemoveFileSpec(lookUp, MAX_PATH);
	wchar_t currentPath[MAX_PATH] = { 0 };
	wchar_t* context = nullptr;

	wchar_t* token = wcstok_s(lookUp, L"\\/", &context);
	while (token != nullptr)
	{
		if (wcslen(currentPath) > 0)
		{
			wcscat_s(currentPath, MAX_PATH, L"\\");
		}
		wcscat_s(currentPath, MAX_PATH, token);

		if (!PathFileExists(currentPath))
		{
			CreateDirectory(currentPath, nullptr);
		}

		token = wcstok_s(nullptr, L"\\/", &context);
	}

	//PathCchRemoveFileSpec(data, wcslen(data));
	if (!PathFileExists(data))
	{
		if (CreateDirectory(data, 0))
		{

		}
	}

	const int evaluatedFlags = EvaluateOverrideRules(overrideRules);
	
	fileHandle = CreateFile( data, GENERIC_READ | GENERIC_WRITE, openRules, 
		nullptr, evaluatedFlags, FILE_ATTRIBUTE_NORMAL, nullptr
	);

	delete[] data;
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		const String text = Layer::GetSystemLayer()->GetError();
		return FILESTATUS_ERROR;
	}

	if (evaluatedFlags == /*OPEN_ALWAYS*/ 4)
	{
		bWasCreatedByProgramatically = (GetLastError() == /*ERROR_SUCCESS*/ 0);
	}
	else if (evaluatedFlags == /*CREATE_NEW*/ 1)
	{
		bWasCreatedByProgramatically = (GetLastError() != /*ERROR_FILE_EXISTS*/ 80);
	}

	return FILESTATUS_GOOD;
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

int WindowsFile::EvaluateOverrideRules(FileOverrideRules flags) const
{
	switch (flags)
	{
	case OVERRIDERULE_JUST_OPEN: return /*OPEN_ALWAYS*/ 4;
	case OVERRIDERULE_OPEN_ONLY_IF_EXISTS:	return /*OPEN_EXISTING*/ 3;
	case OVERRIDERULE_CREATE_NEW_IF_NOT_EXISTS:	return /*CREATE_NEW*/ 1;
	case OVERRIDERULE_CREATE_NEW_DONT_MIND:	return /*CREATE_ALWAYS*/ 2;
	default:
		break;
	}

	return -1;
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
		if (OSLayer* Layer = Layer::GetSystemLayer())
		{
			const String a = Layer->GetError();
			MR_LOG(LogFileSystem, Error, "WriteFile returned: %s", a.Chr());
			return;
		}

		MR_LOG(LogFileSystem, Error, "WriteFile error!");
	}
}

void WindowsFile::Read()
{
	LARGE_INTEGER lg;
	if (GetFileSizeEx(fileHandle, &lg) == 0)
	{
		if (OSLayer* Layer = Layer::GetSystemLayer())
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
			if (OSLayer* Layer = Layer::GetSystemLayer())
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
		//		if (OSLayer* Layer = Layer::GetSystemLayer())
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
