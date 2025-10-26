/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Module.h"
#include <Platform/FileManager.h>
#include "ModuleProcessor.h"

#include <Windows/Windows.h>
#include <PathCch.h>
#include <Layers/SystemLayer.h>
#include "Finder.h"
#include <Platform/Platform.h>

#pragma comment(lib, "Pathcch.lib")


Module* Module::CreateModule(String* fullPathToModule)
{
	MR_ASSERT(!fullPathToModule->IsEmpty(), "Module path is empty!");
	
	Module* super = new Module();

	super->fullPath = *fullPathToModule;

	ScopedPtr<wchar_t> path = Platform::ConvertToWide(*fullPathToModule);
	PathCchRemoveFileSpec(path.Get(), wcslen(path.Get()));
	
	Finder::ListDirectory(path.Get(), super->includedSources);
	
	uint32_t s = super->includedSources.GetSize();
	for (uint32_t i = 0; i < s; i++)
	{
		if (FileManager::IsEndingWith(super->includedSources[i], "mrbuild"))
		{
			super->includedSources.Remove(i);
			break;
		}
	}

	ModuleProcessor mpr;
	if (mpr.OpenScript(fullPathToModule))
	{
		mpr.ParseScript(mpr.GetBuffer());
	}
	
	return super;
}

void Module::OpenPath(const String& fullPathToModule)
{

}
