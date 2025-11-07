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

ECharacterType Utils::GetCharacterType(const char*& str)
{
	if (!str) return None;

	while (isspace(*str))
		str++;

	const char* constStr = str;
	if (isalpha(*constStr))
	{
		
		while (!isspace(*constStr))
			constStr++;

		return Word;
	}

	if (*constStr == '{')
	{
		return OpenBrace;
	}
	else if (*constStr == '}')
	{
		return ClosedBrace;
	}
	else if (*constStr == ',')
	{
		return Comma;
	}
	else if (*constStr == ';')
	{
		
	}
	else if (*constStr == ':')
	{
		return Colon;
	}

	return None;
}

void Utils::SkipCharacterType(const char*& str, ECharacterType type)
{
	if (str)
	{
		switch (type)
		{
		case Word:
		{
			if (isalpha(*str))
			{
				while (!isspace(*str))
					str++;

				return;
			}
			break;
		}
		default:
			if (Utils::GetCharacterType(str) == type)
				str++;

			break;
		}
	}
}

String Utils::GetWord(const char*& in, bool bStep)
{
	while (isspace(int(*in)))
		in++;

	const char* begin = in;
	const char* end = nullptr;
	uint32_t chars = 0;

	if (*begin == '"')
	{
		begin++; // "
		end = begin;

		while (*end && /*isalpha(*end)*/ *end != '"')
		{
			chars++;
			end++;
		}
		
		end++;
	}
	else
	{
		end = in;
		while (*end && !isspace(*end) && !ispunct(*end)) end++;

		chars = end - begin;
	}

	String returned(begin, chars);
	if (bStep) in = end;

	return returned;
}

void Utils::SkipWord(const char*& in)
{
	if (in)
	{
		while (*in && !isspace(*in))
			in++;
	}
}

bool Utils::IsSpace(const char* buffer)
{
	return buffer[0] == ' ' ? true : false;
}