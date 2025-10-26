/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "SolutionDescriptor.h"

#include "File.h"
#include "Windows/WindowsFileManager.h"
#include "ModuleProcessor.h"

bool SolutionDescriptor::Finalize(Solution* slnToOutput, String* bufferToWrite)
{
	*bufferToWrite = "\nMicrosoft Visual Studio Solution File, Format Version 12.00\n# Visual Studio Version 17\nVisualStudioVersion = 17.11.35327.3\nMinimumVisualStudioVersion = 10.0.40219.1\n";


	return false;
}

IFile* SolutionDescriptor::OpenProject(String* projectPath)
{
	IFile* newFile;

	newFile = FileManager::CreateFileOperation(
		projectPath,
		OPENMODE_READ,
		SHAREMODE_READ,
		OVERRIDERULE_OPEN_ONLY_IF_EXISTS
		);

	if (!newFile) return nullptr;

	newFile->Read();
	return newFile;
}

bool SolutionDescriptor::ParseDescriptor()
{
	//if (file)
	//{
	//	ModuleProcessor mp;
	//	mp.ParseSolutionDescriptor(file->GetBuffer());

	//	return true;
	//}

	return false;
}
