/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "ScriptParser.h"
#include "Module.h"
#include <Commandlet.h>

void ScriptParser::BeginParse(const IModule& module)
{
	if (IFile* file = module.GetModuleBuffer())
	{
		if (const char* buffer = file->GetBuffer())
		{
			ParseBlock(buffer);
		}
	}
}

const char* ScriptParser::ParseIndex(const char* pos)
{
	if (!pos || !(pos + 1))
		return nullptr;

	return pos + 1;
}

const char* ScriptParser::ParseBlock(const char* pos)
{
	while (*pos)
	{


		pos++;
	}

	return nullptr;
}
