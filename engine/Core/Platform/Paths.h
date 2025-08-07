/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
class String;

struct IPaths
{
	static String GetSystemDirectory();

	static String GetDocumentsDirectory();

	static String GetUserDirectory();
	
	static String GetEngineDirectory();

	static String GetEngineSavedDirectory();

	static String GetSavedGameDirectory();
};


#include <Platform/PlatformLayout.h>

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/WindowsPaths.h>
#endif // MR_PLATFORM_WINDOWS
