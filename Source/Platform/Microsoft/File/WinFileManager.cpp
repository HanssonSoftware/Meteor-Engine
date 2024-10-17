/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#include "WinFileManager.h"

bool WinFileManager::openFile(FileOpenInfo* details)
{
	if (!details)
		return false;

	details->outputFile = CreateFile(
		details->name.Chr(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		nullptr,
		details->openRules == FileOpenRules::OpenIfExists ? OPEN_EXISTING : CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	); 

	return details->outputFile != INVALID_HANDLE_VALUE ? true : false;
}

uint64 WinFileManager::getSize(const String& name) const
{
	if (name.isEmpty())
		return 0;

	HANDLE tempHandle = CreateFile(
		name.Chr(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	if (tempHandle == INVALID_HANDLE_VALUE)
	{
		CloseHandle(tempHandle);
		return 0;
	}

	LARGE_INTEGER size;
	GetFileSizeEx(tempHandle, &size);

	uint64 realSize = size.QuadPart;

	CloseHandle(tempHandle);
	return realSize;
}

WinFileManager::WinFileManager()
{
}

WinFileManager::~WinFileManager()
{
}

bool WinFileManager::closeFile(void*& outputFile)
{
	if (!outputFile)
		return false;

	return CloseHandle(outputFile);
}



