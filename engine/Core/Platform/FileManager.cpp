/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "FileManager.h"
#include <Types/String.h>

static_assert(!std::is_same_v<FileManager, IFileManager>, "File Manager is using BASE type!");

bool IFileManager::CreateDirectory(const String name, bool bForceCreate)
{
	return false;
}

bool IFileManager::DeleteDirectory(const String name, bool bToFullPath)
{
	return false;
}

bool IFileManager::IsPathExists(const String name)
{
	return false;
}

String IFileManager::GetExecutableDirectory()
{
	return String();
}

String IFileManager::NormalizeDir(const String input)
{
	return String();
}

IFile* IFileManager::CreateFileOperation(const String path, int accessType, int sharingMode, int createType, FileStatus& status)
{
	return nullptr;
}
