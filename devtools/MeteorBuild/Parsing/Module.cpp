/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Module.h"
#include <Platform/FileManager.h>
#include "ScriptParser.h"

IModule* IModule::CreateModule(const String fullPathToModule)
{
	MR_ASSERT(!fullPathToModule.IsEmpty(), "fullPathToModule is empty!");

	IModule* super = new IModule();

	super->OpenPath(fullPathToModule);
	ScriptParser::BeginParse(*super);

	return super;
}

void IModule::OpenPath(const String fullPathToModule)
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
