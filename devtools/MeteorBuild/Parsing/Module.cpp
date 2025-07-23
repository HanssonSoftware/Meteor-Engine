/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Module.h"
#include <Platform/FileManager.h>
#include "ScriptParser.h"

#include <Windows/Windows.h>
#include <PathCch.h>
#include <Layers/SystemLayer.h>
#include "LocatingInterface.h"

#pragma comment(lib, "Pathcch.lib")

IModule* IModule::CreateModule(const String fullPathToModule)
{
	MR_ASSERT(!fullPathToModule.IsEmpty(), "Module path is empty!");

	IModule* super = new IModule();

	wchar_t* path = Layer::GetSystemLayer()->ConvertToWide(fullPathToModule);
	PathCchRemoveFileSpec(path, wcslen(path));
	
	std::vector<String> pa;
	Locator::ListDirectory(path, pa);
	delete[] path;
	
	//ScriptParser::BeginParse();

	return super;
}

void IModule::Search(const String& dir, std::vector<String>& array)
{
	WIN32_FIND_DATAW find;

	wchar_t* path = Layer::GetSystemLayer()->ConvertToWide(dir.Chr());
	HANDLE file = FindFirstFileW(wcscat(path, L"\\*"), &find);

	do
	{
		if (find.cFileName[0] == L'.')
			continue;

		switch (find.dwFileAttributes)
		{
		case FILE_ATTRIBUTE_DIRECTORY:
			Search(path, array);
			break;

		case FILE_ATTRIBUTE_ARCHIVE:
			
			break;
		}

	} while (FindNextFileW(file, &find) != 0);

	delete[] path;
}

void IModule::OpenPath(const String& fullPathToModule)
{
	FileStatus stat;
	moduleDescriptor = FileManager::CreateFileOperation(fullPathToModule, OPENMODE_READ, SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS, stat);
	if (!moduleDescriptor)
		return;

	moduleDescriptor->Read();
	if (String::Contains(moduleDescriptor->GetBuffer(), "Project"))
	{
		int j = 4;
	}
}
