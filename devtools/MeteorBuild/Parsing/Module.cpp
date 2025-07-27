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

	super->fullPath = fullPathToModule;

	wchar_t* path = Layer::GetSystemLayer()->ConvertToWide(fullPathToModule);
	PathCchRemoveFileSpec(path, wcslen(path));
	
	Locator::ListDirectory(path, super->includedSources);
	delete[] path;
	
	int s = (int)super->includedSources.size();
	for (int i = 0; i < s; i++)
	{
		if (FileManager::IsEndingWith(super->includedSources[i], "mrbuild"))
		{
			super->includedSources.erase(super->includedSources.begin() + i);
			break;
		}
	}

	if (ScriptParser::OpenScript(super))
	{
		ScriptParser::BeginParse();
	}
	
	return super;
}

void Module::OpenPath(const String& fullPathToModule)
{

}
