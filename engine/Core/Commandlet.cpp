/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Commandlet.h"
#include <Types/Array.h>
#include <Types/String.h>
#include <Platform/PlatformLayout.h>

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/Windows.h>
#endif // MR_PLATFORM_WINDOWS

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

	delete[] cli;
}

bool Commandlet::Parse(const String& inParam, String& returnVal)
{
	for (String& param : parsedWords)
	{
		if (param == inParam)
		{
			returnVal = param;
			return true;
		}
	}

	return false;
}