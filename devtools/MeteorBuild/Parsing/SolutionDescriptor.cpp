/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "SolutionDescriptor.h"

#include "File.h"
#include "Windows/WindowsFileManager.h"

bool SolutionDescriptor::Finalize(String* bufferToWrite)
{
	*bufferToWrite = "\nMicrosoft Visual Studio Solution File, Format Version 12.00\n# Visual Studio Version 17\nVisualStudioVersion = 17.11.35327.3\nMinimumVisualStudioVersion = 10.0.40219.1\n";


	return false;
}

bool SolutionDescriptor::OpenProject(const String& projectPath)
{
	FileStatus stat;
	file = FileManager::CreateFileOperation(
		projectPath,
		OPENMODE_READ,
		SHAREMODE_READ,
		OVERRIDERULE_OPEN_ONLY_IF_EXISTS,
		stat);

	if (!file) return false;

	file->Read();
	return true;
}
