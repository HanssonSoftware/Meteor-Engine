/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/FileManager.h>
#undef CreateDirectory

class IFile;

struct WindowsFileManager : public IFileManager
{
	static bool CreateDirectory(const String name, bool bToFullPath);

	static bool DeleteDirectory(const String name, bool bToFullPath);

	static bool IsPathExists(const String name);

	static String GetExecutableDirectory();

	static String NormalizeDir(const String input);

	static IFile* CreateFileOperation(const String path, int accessType, int sharingMode, FileOverrideRules createType, FileStatus& status);
private:
	static constexpr const int evaluateAccessTypeForCreateFileOperation(int accessType);

	static constexpr const int evaluateSharingModeForCreateFileOperation(int sharingMode);

	static constexpr const int evaluateCreateTypeForCreateFileOperation(FileOverrideRules createType);

	static bool startRecursiveCreate(wchar_t* dir);
};

using FileManager = WindowsFileManager;