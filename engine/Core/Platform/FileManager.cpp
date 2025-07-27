/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "FileManager.h"
#include <Types/String.h>

#pragma warning(disable : 6031)

static_assert(!std::is_same_v<FileManager, IFileManager>, "File Manager is using BASE type!");

bool IFileManager::CreateDirectory(const String& name, bool bForceCreate)
{
	return false;
}

bool IFileManager::DeleteDirectory(const String& name, bool bToFullPath)
{
	return false;
}

void IFileManager::ListDirectory(String directoryToCheck, std::vector<String>& output)
{
}

bool IFileManager::IsPathExists(const String& name)
{
	return false;
}

bool IFileManager::IsPathRelative(const String& path)
{
	return false;
}

bool IFileManager::IsEndingWith(const String& name, const String& extension)
{
	if (name.IsEmpty() || extension.IsEmpty())
		return false;

	char* buffer = _strdup(name.Chr());
	MR_ASSERT(strcmp(buffer, name.Chr()) == 0, "Buffer duplication error!");

	strtok(buffer, ".");
	char* returned = strtok(0, ".");
	if (strcmp(extension.Chr(), returned) != 0)
	{
		free(buffer);
		return false;
	}

	free(buffer);
	return true;
}

String IFileManager::GetExecutableDirectory()
{
	return String();
}

void IFileManager::NormalizeDirectory(String& input)
{
	
}

IFile* IFileManager::CreateFileOperation(const String& path, int accessType, int sharingMode, int createType, FileStatus& status)
{
	return nullptr;
}
