/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "LocatingInterface.h"
#include <Windows/Windows.h>
#include <Platform/FileManager.h>
#include <Layers/OSLayer.h>
#include <PathCch.h>

#pragma comment(lib, "Pathcch.lib")

LOG_ADDCATEGORY(LocatingInterface);

static bool bIsFirstCall = true;

bool LocatingInterface::FindAllReferences(const String sourceDirectory)
{
    MR_ASSERT(!(!sourceDirectory), "Directory is empty! Consider checking via Debugger!");
    
    String fullPath = sourceDirectory;
    if (bIsFirstCall)
    {
        bIsFirstCall = false;

        if (FileManager::IsPathRelative(sourceDirectory))
        {
            fullPath = FileManager::GetExecutableDirectory();
            if (OSLayer* systemLayer = Layer::GetSystemLayer())
            {
                wchar_t* wideExecutableDirectory = systemLayer->ConvertToWide(fullPath.Chr());
                
                PathCchRemoveFileSpec(wideExecutableDirectory, wcslen(wideExecutableDirectory));
                fullPath = String::Format("%ls\\%s", wideExecutableDirectory, sourceDirectory.Chr());
                wchar_t* wideExecutableDirectoryNoFile = systemLayer->ConvertToWide(fullPath.Chr());

                wchar_t* canonicalizedPath = nullptr;
                if (PathAllocCanonicalize(wideExecutableDirectoryNoFile, PATHCCH_ALLOW_LONG_PATHS, &canonicalizedPath) != S_OK)
                {
                    MR_LOG(LogLocatingInterface, Fatal, "PathAllocCanonicalize returned: %s", systemLayer->GetError().Chr());
                }
                
                fullPath = canonicalizedPath;

                LocalFree(canonicalizedPath);
                delete[] wideExecutableDirectory;
            }
        }
        else
        {
            if (!FileManager::IsPathExists(sourceDirectory))
            {
                MR_LOG(LogLocatingInterface, Fatal, "Directory does not exist! (%s)", sourceDirectory.Chr());
            }
        }
    }

    if (!FileManager::IsPathExists(fullPath))
    {
        MR_LOG(LogLocatingInterface, Fatal, "Directory does not exist! (%s)", sourceDirectory.Chr());
    }

    if (OSLayer* systemLayer = Layer::GetSystemLayer())
    {
        wchar_t* path = systemLayer->ConvertToWide(fullPath.Chr());

        wchar_t* pathWAserisk = systemLayer->ConvertToWide(String::Format("%s\\*", fullPath.Chr()));
        WIN32_FIND_DATAW searchData;
        HANDLE file = FindFirstFileW(pathWAserisk, &searchData);

        delete[] pathWAserisk;
        if (file != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (searchData.cFileName[0] == L'.')
                    continue;


                if (searchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    const String newPath = String::Format("%ls\\%ls", path, searchData.cFileName);
                    FindAllReferences(newPath);
                }
                else if (searchData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
                {
                    if (FileManager::IsEndingWith(searchData.cFileName, "mrbuild"))
                    {
                        MR_LOG(LogLocatingInterface, Verbose, "Module descriptor found: %ls\\%ls", path, searchData.cFileName);

                        IModule::CreateModule(String::Format("%ls\\%ls", path, searchData.cFileName));
                    }
                }

            } while (FindNextFileW(file, &searchData) != 0);

            FindClose(file);
        }

        delete[] path;
        return true;
    }

    return false;
}
