/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Module.h"
#include <Platform/FileManager.h>
#include "ScriptParser.h"

#include <Windows/Windows.h>
#include <PathCch.h>
#include <Layers/SystemLayer.h>
#include "LocatingInterface.h"

#pragma comment(lib, "Pathcch.lib")

Module* Module::CreateModule(const String fullPathToModule)
{
	MR_ASSERT(!fullPathToModule.IsEmpty(), "Module path is empty!");

	Module* super = new Module();

	wchar_t* path = Layer::GetSystemLayer()->ConvertToWide(fullPathToModule);
	PathCchRemoveFileSpec(path, wcslen(path));
	
	std::vector<String> pa;
	Locator::ListDirectory(path, pa);
	delete[] path;
	
	int s = pa.size();
	for (int i = 0; i < s; i++)
	{
		if (FileManager::IsEndingWith(pa[i], "mrbuild"))
		{
			pa.erase(pa.begin() + i);
			break;
		}
	}

	//ScriptParser::BeginParse();

	return super;
}

void Module::OpenPath(const String& fullPathToModule)
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
