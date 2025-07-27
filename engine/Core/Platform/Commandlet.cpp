/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Commandlet.h"
#include <Platform/PlatformLayout.h>

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/Windows.h>
#endif // MR_PLATFORM_WINDOWS

#pragma warning(disable : 6031)

static const char* commandLine;

void ICommandlet::Initalize()
{
#ifdef MR_PLATFORM_WINDOWS
	commandLine = GetCommandLineA();
#endif // MR_PLATFORM_WINDOWS
}

String ICommandlet::Parse(const char* inParam)
{
	MR_ASSERT(commandLine, "CLI buffer is empty, consider checking!");

	static String cachedValue;
	if (!inParam)
	{
		return cachedValue;
	}

	char* cmd = _strdup(commandLine);

	MR_ASSERT(strcmp(cmd, commandLine) == 0, "Buffer duplication error!");

	char* deconstedParam = const_cast<char*>(inParam);

	char* found = strstr(cmd, deconstedParam);
	if (found)
	{
		char* foundCommand = strtok(found, " ");

		char* foundParam = strtok(0, " ");
		if (!foundParam || foundParam[0] == '-')
		{
			cachedValue = "1";
			free(cmd);

			return "1";
		}

		cachedValue = foundParam;
		free(cmd);

		return /* foundParam */ cachedValue;
	}

	free(cmd);
	return "";
}

String ICommandlet::Parse(const char* buffer, const char* inParam)
{
	if (!buffer || !inParam)
		return "";

	char* cmd = _strdup(buffer);

	MR_ASSERT(strcmp(cmd, buffer) == 0, "Buffer duplication error!");

	char* deconstedParam = const_cast<char*>(inParam);

	char* found = strstr(cmd, deconstedParam);
	if (found)
	{
		strtok(found, " ");

		char* foundParam = strtok(0, " ");

		String c = foundParam;
		free(cmd);
		return c;
	}

	free(cmd);
	return "";
}
