/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "File.h"
#include <string>
#include <Logging/LogMacros.h>

#ifdef _WIN64
#include <Windows/WindowsFile.h>
#endif // _WIN64

IFile* CreateFileOperation()
{
#ifdef _WIN64
	return new WindowsFile; 
#endif // _WIN64
}

IFile::~IFile()
{
	if (buffer != nullptr)
	{
		delete[] buffer;
		buffer = nullptr;
	}
}

FileStatus IFile::Open(const String Name, int openRules, FileOverrideRules overrideRules)
{
	if (Name.IsEmpty())
	{
		MR_LOG(LogFileSystem, Error, "How is the File Supposed to be Named?");
		return FILESTATUS_ERROR;
	}

	return FILESTATUS_GOOD;
}

void IFile::Delete()
{
	if (!bWasInitSucceded)
		return;
}

void IFile::Write(const String buffer) const
{
	if (!bWasInitSucceded)
		return;
}

void IFile::Read()
{
	if (!bWasInitSucceded)
		return;
}
