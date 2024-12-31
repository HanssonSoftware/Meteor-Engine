/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

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
		FlushFileBuffers(fileHandle);
		CloseHandle(fileHandle);
	}

#endif // _WIN32

	if (buffer != nullptr)
	{
		mrfree(buffer);
	}
}

File::File(const String Name, int openRules, FileOverrideRules overrideRules)
{
	if (Name.isEmpty())
	{
		MR_LOG(LogFileSystem, Error, TEXT("How is the File Supposed to be Named?"));
		return;
	}

#ifdef _WIN32
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
		Logger::Get().dispatchLastError();
		return;
	}

#endif // _WIN32
	String name = Name;
	String ext = Name;
	fileName = name.Delim(".", true);
	extension = ext.Delim(".", false);
	bWasInitSucceded = true;
}

void File::Write(const String buffer) const
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

	const int& requestedNarrowQuantity = WideCharToMultiByte(CP_UTF8, 0, buffer.Chr(), -1, 0, 0, 0, 0);
	char* bufferA = (char*)mrmalloc(requestedNarrowQuantity);

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
		mrfree(bufferA);

#endif // _WIN32
}

void File::Read()
{
	if (!bWasInitSucceded)
		return;

	if (!fileHandle)
	{
		MR_LOG(LogFileSystem, Error, TEXT("Unable to Read to Buffer!"));
		return;
	}

#ifdef _WIN32
	LARGE_INTEGER lg;
	GetFileSizeEx(fileHandle, &lg);
	DWORD write = 1;

	char* narrowBuffer = nullptr;
	if (lg.QuadPart > 0)
		narrowBuffer = (char*)mrmalloc((lg.QuadPart + 1) * sizeof(char));
	else
		narrowBuffer = (char*)mrmalloc(sizeof(char));

	if (!ReadFile(fileHandle, (CHAR*)narrowBuffer, (DWORD)(lg.QuadPart + 1), &write, 0))
	{
		Logger::Get().dispatchLastError();
	}

	const int& requiredAmount = MultiByteToWideChar(CP_UTF8, 0, narrowBuffer, -1, 0, 0);
	buffer = (wchar_t*)mrmalloc((requiredAmount) * sizeof(wchar_t));
	
	MultiByteToWideChar(CP_UTF8, 0, narrowBuffer, -1, buffer, requiredAmount);

	mrfree(narrowBuffer);
#endif
	String superBuffer(buffer);
}


void File::setName(String content)
{
	if (content.isEmpty())
	{
		MR_LOG(LogFileSystem, Error, TEXT("How is the File Supposed to be Named?"));
		return;
	}

	if (fileName == content)
	{
		MR_LOG(LogFileSystem, Error, TEXT("Renaming as the Same as Before? This won't Happen."));
		return;
	}

	const String contentBackup = content;
	String a = content.Delim(".", true);
	if (a == contentBackup)
	{
		content = String::Format("%s.%s", a, extension);
	}

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

inline constexpr const int File::evaluateOverrideRules(FileOverrideRules flags)
{
	switch (flags)
	{
	case OVERRIDERULE_JUST_OPEN:
#ifdef _WIN32
		return 4;
#else

#endif // _WIN32
		break;
	case OVERRIDERULE_OPEN_ONLY_IF_EXISTS:
#ifdef _WIN32
		return 3;
#else

#endif // _WIN32
		break;
	case OVERRIDERULE_CREATE_NEW_IF_NOT_EXISTS:
#ifdef _WIN32
		return 1;
#else

#endif // _WIN32
		break;
	case OVERRIDERULE_CREATE_NEW_DONT_MIND:
#ifdef _WIN32
		return 2;
#else

#endif // _WIN32
		break;
	default:
		break;
	}

	return -1;
}