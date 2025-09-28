/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Pointers.h>
#include <Types/String.h>

struct IPlatform
{
	static ScopedPtr<wchar_t> ConvertToWide(const char* skinny) {};

	static ScopedPtr<char> ConvertToNarrow(const wchar_t* fat) {};

	static String GetError() {};
};


#include "PlatformLayout.h"

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/WindowsPlatform.h>
#endif // MR_PLATFORM_WINDOWS
