/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#include "FileManager.h"
#ifdef _WIN32
#include <Microsoft/File/WinFileManager.h>
#endif // _WIN32


IFileManager& IFileManager::Get()
{
#ifdef _WIN32
	static WinFileManager instance;
#else defined(LNX)
	static LnxFileManager instance;
#endif // _WIN32
	return instance;
}

IFileManager::IFileManager() noexcept
{
}

IFileManager::~IFileManager()
{
}
