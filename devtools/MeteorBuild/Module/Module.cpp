/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Module.h"
#include <FileManager.h>
#include <Platform/File.h>
#include <Core/Utils.h>

void Module::Parse(String* modulePath)
{
	IFile* module = FileManager::CreateFileOperation(modulePath, FileAccessMode::OPENMODE_READ, FileShareMode::SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS);
	if (module != nullptr)
	{
		module->Read();

		const char* buffer = module->GetBuffer();

		while (buffer != "\0")
		{
			Utils::GetCharacterType(buffer);
			buffer++;
		}

		module->Close();
	}
}
