/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "ScriptParser.h"
#include <Logging/LogMacros.h>
#include "Module.h"
#include <FileManager.h>
#include <Commandlet.h>

LOG_ADDCATEGORY(Script);

void ScriptParser::BeginParse()
{
	if (IFile* file = currentlyReadModule->GetModuleBuffer())
	{
		buffer = new char[file->GetSize() + 1]();
		memcpy(buffer, file->GetBuffer(), file->GetSize());

		Expected("Project");
		SkipWhitspace();

		GetValue();
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

	currentlyReadModule = module;
	return true;
}

void ScriptParser::Expected(const char* ptr)
{
	const char* begin = buffer;
	while (IsAlpha(*buffer)) buffer++;

	String temp(begin, buffer - begin);
	if (strcmp(temp.Chr(), ptr) > 0)
	{
		MR_LOG(LogScript, Fatal, "Failed to parse! Word: %s At: Char[%l]", ptr, buffer - begin);
	}
}

void ScriptParser::GetValue()
{
	buffer++;

	const char* begin = buffer;
	while (IsAlpha(*buffer)) buffer++;

	String val(begin, buffer - begin);

	if (*buffer == '"')
		buffer++;
}

void ScriptParser::AdvanceACharacter()
{
	if (*buffer)
	{
		buffer++;
	}
}

void ScriptParser::SkipWhitspace()
{
	while (*buffer == ' ' || *buffer == '\n' || *buffer == '\r' || *buffer == '\t' || *buffer == '\v')
		buffer++;

}

bool ScriptParser::IsWhitspace()
{
	return (*buffer == ' ' || *buffer == '\n' || *buffer == '\r' || *buffer == '\t' || *buffer == '\v') ? true : false;	
}

bool ScriptParser::IsAlpha(const char input)
{
	return (input >= 'a' && input <= 'z' || input >= 'A' && input <= 'Z');
}
