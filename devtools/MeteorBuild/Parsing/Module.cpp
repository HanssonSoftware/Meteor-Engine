/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Module.h"
#include <Platform/FileManager.h>
#include "ModuleProcessor.h"

#include <Windows/Windows.h>
#include <PathCch.h>
#include <Layers/SystemLayer.h>
#include "LocatingInterface.h"

#pragma comment(lib, "Pathcch.lib")


Module* Module::CreateModule(const String& fullPathToModule)
{
	MR_ASSERT(!fullPathToModule.IsEmpty(), "Module path is empty!");

	static bool bIsMainModuleCreated = false;
	Module* super = new Module();

	super->fullPath = fullPathToModule;

	wchar_t* path = Layer::GetSystemLayer()->ConvertToWide(fullPathToModule);
	PathCchRemoveFileSpec(path, wcslen(path));
	
	Locator::ListDirectory(path, super->includedSources);
	delete[] path;
	
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
		mpr.ParseScript(mpr.GetBuffer(), bIsMainModuleCreated ? ModuleProcessor::ParsingType::Module : ModuleProcessor::ParsingType::MainDescriptor);
	}
	
	bIsMainModuleCreated = true;
	return super;
}

void Module::OpenPath(const String& fullPathToModule)
{

}
