/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Logging/LogMacros.h>
#include <Platform/FileHelpers.h>
#undef CreateDirectory

#include <vector>

class String;

LOG_ADDCATEGORY(FileManager);

struct IFileManager
{
	static bool CreateDirectory(const String& name, bool bForceCreate);

	static bool DeleteDirectory(const String& name, bool bToFullPath);
	
	static bool IsPathExists(const String& name);

	static bool IsPathRelative(const String& path);

	static bool IsEndingWith(const String& name, const String& extension);

	static void NormalizeDirectory(String& input);

	static IFile* CreateFileOperation(const String& path, int accessType, int sharingMode, int createType, FileStatus& status);

private:
	IFileManager() = default;
};

#include "PlatformLayout.h"

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/WindowsFileManager.h>
#endif // DEBUG
