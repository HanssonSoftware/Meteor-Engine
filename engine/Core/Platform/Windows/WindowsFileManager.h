/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Platform/FileManager.h>
#undef CreateDirectory

#include <Types/String.h>

class IFile;

struct WindowsFileManager : public IFileManager
{
	static bool CreateDirectory(const String& name, bool bToFullPath);

	static bool DeleteDirectory(const String& name, bool bToFullPath);

	static bool IsPathExists(const String& name);

	/** Returns true if the path is qualified, or false otherwise. */
	static bool IsPathRelative(const String& path);

	static bool IsEndingWith(const String& name, const String& extension);

	static void NormalizeDirectory(String& input);

	static IFile* CreateFileOperation(const String& path, int accessType, int sharingMode, FileOverrideRules createType, FileStatus& status);
private:
	WindowsFileManager() = default;

	static constexpr const int evaluateAccessTypeForCreateFileOperation(int accessType);

	static constexpr const int evaluateSharingModeForCreateFileOperation(int sharingMode);

	static constexpr const int evaluateCreateTypeForCreateFileOperation(FileOverrideRules createType);

	static bool startRecursiveCreate(wchar_t* dir);
};

using FileManager = WindowsFileManager;