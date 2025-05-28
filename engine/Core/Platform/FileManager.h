/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Logging/LogMacros.h>
#include <Platform/FileHelpers.h>
#undef CreateDirectory

struct String;
class File;

LOG_ADDCATEGORY(FileManager);

struct IFileManager
{
	static bool CreateDirectory(const String name, bool bForceCreate);

	static bool DeleteDirectory(const String name, bool bToFullPath);

	static bool IsPathExists(const String name);

	static String GetExecutableDirectory();

	static String NormalizeDir(const String input);

	static IFile* CreateFileOperation(const String path, int accessType, int sharingMode, int createType, FileStatus& status);
};

#include "PlatformLayout.h"

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/WindowsFileManager.h>
#endif // DEBUG
