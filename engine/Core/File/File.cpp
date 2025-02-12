/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "File.h"
#include <string>
#include <Platform/Microsoft/MinWin.h>
#include <Log/Exception.h>
#include <Common/MemoryManager.h>

#ifdef _WIN32
#include <Windows.h>
#include <pathcch.h>
#pragma comment(lib, "Pathcch.lib")
#include <shlwapi.h>
#include <winbase.h>
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Kernel32.lib")
#endif // _WIN32

FMFile::FMFile()
{
	buffer = nullptr;
}

FMFile::~FMFile()
{
#ifdef _WIN32

	if (fileHandle)
	{
		FlushFileBuffers(fileHandle);
		CloseHandle(fileHandle);
	}

#endif // _WIN32

	if (buffer != nullptr)
	{
		delete[] buffer;
	}
}

FileStatus FMFile::Open(const String Name, int openRules, FileOverrideRules overrideRules)
{
	if (Name.isEmpty())
	{
		MR_LOG(LogFileSystem, Error, TEXT("How is the File Supposed to be Named?"));
		return FILESTATUS_ERROR;
	}

#ifdef _WIN32

	String g = Name;
	PathCchRemoveFileSpec(g.Data(), wcslen(g.Chr()));
	if (!PathFileExists(g.Chr()))
	{
		if (CreateDirectory(g.Chr(), 0))
		{

		}
	}

	fileHandle = CreateFile(
		Name.Chr(),
		GENERIC_READ | GENERIC_WRITE,
		openRules,
		nullptr,
		evaluateOverrideRules(overrideRules),
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		Logger::Get().getLastError();
		return FILESTATUS_ERROR;
	}

	if (evaluateOverrideRules(overrideRules) == /*OPEN_ALWAYS*/ 4)
	{
		bWasCreatedByProgramatically = (GetLastError() == /*ERROR_SUCCESS*/ 0);
	}
	else if (evaluateOverrideRules(overrideRules) == /*CREATE_NEW*/ 1)
	{
		bWasCreatedByProgramatically = (GetLastError() != /*ERROR_FILE_EXISTS*/ 80);
	}


#endif // _WIN32
	String name = Name;
	String ext = Name;
	fileName = name.Delim(".", true);
	extension = ext.Delim(".", false);
	bWasInitSucceded = true;
	return FILESTATUS_GOOD;
}

void FMFile::Delete()
{
	if (!bWasInitSucceded)
		return;

	if (buffer)
		delete[] buffer;

	if (!DeleteFile(fileName.Chr()))
		THROW_EXCEPTION("Unable To Delete File! %s", fileName.Chr());
}

void FMFile::Write(const String buffer) const
{
	if (!bWasInitSucceded)
		return;

	if (!fileHandle)
	{
		MR_LOG(LogFileSystem, Error, TEXT("Invalid Handle!"));
		return;
	}

	if (buffer.isEmpty())
	{
		MR_LOG(LogFileSystem, Error,TEXT("Buffer is Empty!"));
		return;
	}

#ifdef _WIN32

	DWORD charsWritten = 0;

	const int requestedNarrowQuantity = WideCharToMultiByte(CP_UTF8, 0, buffer.Chr(), -1, 0, 0, 0, 0);
	char* bufferA = new char[requestedNarrowQuantity];

	WideCharToMultiByte(CP_UTF8, 0, buffer.Chr(), -1, bufferA, requestedNarrowQuantity, 0, 0);

	if (WriteFile(fileHandle, (CHAR*)bufferA, (DWORD)requestedNarrowQuantity - 1, &charsWritten, 0))
	{
		FlushFileBuffers(fileHandle);
	}
	else
	{
		MR_LOG(LogFileSystem, Error, TEXT("Failed Writing to File! %s"), getName().Chr());
	}

	if (bufferA)
		delete[] bufferA;

#endif // _WIN32
}

void FMFile::Read()
{
	if (!bWasInitSucceded)
		return;

	if (!fileHandle)
		THROW_EXCEPTION("File Handle Invalid!");

#ifdef _WIN32

	LARGE_INTEGER lg;
	if (GetFileSizeEx(fileHandle, &lg) == 0)
	{
		const String a = getLastError();
		MR_LOG(LogFileSystem, Error, TEXT("GetFileSizeEx returned: %s"), a.Chr());
		return;
	}

	DWORD write = 0;

	char* narrowBuffer;
	if (lg.QuadPart > 0)
		narrowBuffer = new char[lg.QuadPart + 1];
	else
		narrowBuffer = new char[1];


	if (narrowBuffer)
	{
		if (!ReadFile(fileHandle, narrowBuffer, (DWORD)lg.QuadPart, &write, 0))
			Logger::Get().getLastError();

		narrowBuffer[write] = '\0';

		const int requiredAmount = MultiByteToWideChar(CP_UTF8, 0, narrowBuffer, write, 0, 0);
		buffer = new wchar_t[requiredAmount + 1];

		const int writtenAmount = MultiByteToWideChar(CP_UTF8, 0, narrowBuffer, write, buffer, requiredAmount);
		if (writtenAmount == 0 || writtenAmount != requiredAmount)
		{
			const String a = getLastError();
			MR_LOG(LogFileSystem, Error, TEXT("MultiByteToWideChar returned: %s"), a.Chr());
		}

		buffer[write] = L'\0';
		delete[] narrowBuffer;
	}
	else
		THROW_EXCEPTION("Invalid Buffer!");
#endif
}


void FMFile::setName(String content)
{
	if (content.isEmpty())
		THROW_EXCEPTION("How is the File Supposed to be Named?");

	if (fileName == content)
		THROW_EXCEPTION("Renaming as the Same as Before? This won't Happen.");

	const String contentBackup = content;
	String a = content.Delim(".", true);
	if (a == contentBackup)
		content = String::Format("%s.%s", a, extension);

#ifdef _WIN32
	if (MoveFile(String::Format("%s.%s", fileName.Chr(), extension.Chr()).Chr(), contentBackup.Chr()))
	{
		String contentA = contentBackup;
		fileName = contentA.Delim(".", true);
		contentA = contentBackup;
		extension = contentA.Delim(".", false);
	}
#endif // _WIN32
}

constexpr int FMFile::evaluateOverrideRules(FileOverrideRules flags)
{
	switch (flags)
	{
	case OVERRIDERULE_JUST_OPEN:
#ifdef _WIN32
		return /*OPEN_ALWAYS*/ 4;
#else

#endif // _WIN32
		break;
	case OVERRIDERULE_OPEN_ONLY_IF_EXISTS:
#ifdef _WIN32
		return /*OPEN_EXISTING*/ 3;
#else

#endif // _WIN32
		break;
	case OVERRIDERULE_CREATE_NEW_IF_NOT_EXISTS:
#ifdef _WIN32
		return /*CREATE_NEW*/ 1;
#else

#endif // _WIN32
		break;
	case OVERRIDERULE_CREATE_NEW_DONT_MIND:
#ifdef _WIN32
		return /*CREATE_ALWAYS*/ 2;
#else

#endif // _WIN32
		break;
	default:
		break;
	}

	return -1;
}


