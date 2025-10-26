/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Commandlet.h"
#include <Types/Array.h>
#include <Types/String.h>
#include <Platform/PlatformLayout.h>

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/Windows.h>
#endif // MR_PLATFORM_WINDOWS
#include <MemoryManager.h>

#pragma warning(disable : 6031)

void Commandlet::Initialize()
{
	String commandLine;
#ifdef MR_PLATFORM_WINDOWS
	commandLine = GetCommandLineW();
#endif // MR_PLATFORM_WINDOWS

	char* cli = commandLine.Allocate();
	
	char* token = strtok(cli, " ");
	while (token != nullptr)
	{
		parsedWords.Add(String(token));
		token = strtok(nullptr, " ");
	}

	MemoryManager::Get().Deallocate(cli);
}

bool Commandlet::Parse(const String& inParam, String& returnVal)
{
	static uint32_t count = parsedWords.GetSize();

	for (uint32_t i = 0; i < count; i++)
	{
		String& item = parsedWords[i];

		if (item == inParam)
		{
			returnVal = parsedWords.IsOutOfBounds(i + 1) ? parsedWords[i] : parsedWords[i + 1];
			return true;
		}
	}

	return false;
}