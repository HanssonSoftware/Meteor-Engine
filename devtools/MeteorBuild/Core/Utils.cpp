/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Utils.h"
#include <FileManager.h>
#include <Platform/Paths.h>
#include <Types/Pointers.h>
#include <Platform.h>

#include <PathCch.h>

void Utils::ListDirectory(String* name, Array<String>& container)
{
	if (name != nullptr)
	{
		if (FileManager::IsPathRelative(name))
		{
			String exeDir = Paths::GetExecutableDirctory();

			ScopedPtr<wchar_t> exeDirWide = Platform::ConvertToWide(exeDir);
			PathCchRemoveFileSpec(exeDirWide.Get(), wcslen(exeDirWide));

			ScopedPtr<wchar_t> nameWide = Platform::ConvertToWide(*name);

			PWSTR combinedPathNonCanonicalized;
			PathAllocCombine(exeDirWide, nameWide, PATHCCH_ALLOW_LONG_PATHS, &combinedPathNonCanonicalized);

			*name = combinedPathNonCanonicalized;
			LocalFree(combinedPathNonCanonicalized);
		}

		WIN32_FIND_DATAW foundFile;
		ScopedPtr<wchar_t> widePath = Platform::ConvertToWide(*String::Format("%s\\*", name->Chr()));

		HANDLE fileHandle = FindFirstFileW(widePath, &foundFile);

		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (foundFile.cFileName[0] == '.')
					continue;

				switch (foundFile.dwFileAttributes)
				{
					case FILE_ATTRIBUTE_DIRECTORY:
					{
						String nextDirectory = String::Format("%s\\%ls", name->Chr(), foundFile.cFileName);
						ListDirectory(&nextDirectory, container);
						break;
					};

					case FILE_ATTRIBUTE_ARCHIVE:
					{
						container.Add(String::Format("%s\\%ls", name->Chr(), foundFile.cFileName));
						break;
					}
				}

			} while (FindNextFileW(fileHandle, &foundFile));
		
			FindClose(fileHandle);
		}

	}
}

ECharacterType Utils::GetCharacterType(const char* str)
{
	if (!str) return None;

	char* c = const_cast<char*>(str);
	if (isalpha((int)*c))
	{
		int i = 0;
		while (!isspace((int)*c))
		{


			i++;
			c++;
		}
	}

	if (str[0] == '{')
	{
		return OpenBrace;
	}
	else if (str[0] == '}')
	{
		return ClosedBrace;
	}
	else if (str[0] == ',')
	{
		return Comma;
	}
	else if (str[0] == ';')
	{
		
	}
	else if (str[0] == ':')
	{
		return Colon;
	}

	return None;
}

bool Utils::IsSpace(const char* buffer)
{
	return buffer[0] == ' ' ? true : false;
}