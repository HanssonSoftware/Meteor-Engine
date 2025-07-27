/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "ScriptParser.h"
#include "Module.h"
#include <FileManager.h>
#include <Commandlet.h>

void ScriptParser::BeginParse()
{
	if (IFile* file = currentlyReadModule->GetModuleBuffer())
	{
		if (const char* buffer = file->GetBuffer())
		{
			ParseBlock(buffer);
		}
	}
}

bool ScriptParser::OpenScript(Module* module)
{
	if (!module) return false;

	FileStatus stat;
	IFile*& fileDescriptor = module->moduleDescriptor;

	fileDescriptor = FileManager::CreateFileOperation(module->fullPath, OPENMODE_READ, SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS, stat);
	if (!fileDescriptor)
		return false;

	fileDescriptor->Read();
	return true;
}

void ScriptParser::AdvanceACharacter()
{
	if (position != '\0')
	{
		position++;
	}
}

void ScriptParser::SkipWhitspace()
{
}
