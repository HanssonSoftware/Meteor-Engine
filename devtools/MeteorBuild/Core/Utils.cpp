/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Utils.h"
#include <Platform/FileManager.h>
#include <Platform/Paths.h>
#include <Types/Pointers.h>
#include <Platform/Platform.h>

#include <PathCch.h>
#include "Parser.h"

void Utils::ListDirectory(String* name, Array<FoundScriptData>& container)
{
	if (name != nullptr)
	{
		if (FileManager::IsPathRelative(name))
		{
			String exeDir = Paths::GetExecutableDirctory();

			PathCchRemoveFileSpec(exeDir.Data(), exeDir.Length());

			PWSTR combinedPathNonCanonicalized;
			PathAllocCombine(exeDir.Data(), *name, PATHCCH_ALLOW_LONG_PATHS, &combinedPathNonCanonicalized);

			*name = combinedPathNonCanonicalized;
			LocalFree(combinedPathNonCanonicalized);
		}

		WIN32_FIND_DATAW foundFile;
		HANDLE fileHandle = FindFirstFileW(String::Format("%ls\\*", name->Chr()), &foundFile);

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
						String nextDirectory = String::Format("%ls\\%ls", name->Chr(), foundFile.cFileName);
						ListDirectory(&nextDirectory, container);
						break;
					};

					case FILE_ATTRIBUTE_ARCHIVE:
					{
						FoundScriptData fd;

						fd.name = foundFile.cFileName;
						fd.path = *name;
						fd.full = String::Format("%ls\\%ls", *fd.path, *fd.name);

						container.Add(fd);
						break;
					}
				}

			} while (FindNextFileW(fileHandle, &foundFile));
		
			FindClose(fileHandle);
		}
	}
}

