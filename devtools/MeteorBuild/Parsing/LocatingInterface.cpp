/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "LocatingInterface.h"
#include <Types/String.h>
#include <Windows/Windows.h>
#include <Platform/FileManager.h>
#include <Layers/SystemLayer.h>
#include <PathCch.h>

#pragma comment(lib, "Pathcch.lib")

LOG_ADDCATEGORY(Locator);

static bool bIsFirstCall = true;

bool Locator::FindAllReferences(const String& sourceDirectory)
{
    MR_ASSERT(!(!sourceDirectory), "Directory is empty! Consider checking via Debugger!");
    
    String fullPath = sourceDirectory;
    if (bIsFirstCall)
    {
        bIsFirstCall = false;

        if (FileManager::IsPathRelative(sourceDirectory))
        {
            fullPath = FileManager::GetExecutableDirectory();
            if (SystemLayer* systemLayer = Layer::GetSystemLayer())
            {
                wchar_t* wideExecutableDirectory = systemLayer->ConvertToWide(fullPath.Chr());
                
                PathCchRemoveFileSpec(wideExecutableDirectory, wcslen(wideExecutableDirectory));
                fullPath = String::Format("%ls\\%s", wideExecutableDirectory, sourceDirectory.Chr());
                wchar_t* wideExecutableDirectoryNoFile = systemLayer->ConvertToWide(fullPath.Chr());

                wchar_t* canonicalizedPath = nullptr;
                if (PathAllocCanonicalize(wideExecutableDirectoryNoFile, PATHCCH_ALLOW_LONG_PATHS, &canonicalizedPath) != S_OK)
                {
                    MR_LOG(LogLocator, Fatal, "PathAllocCanonicalize returned: %s", systemLayer->GetError().Chr());
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
                MR_LOG(LogLocator, Fatal, "Directory does not exist! (%s)", sourceDirectory.Chr());
            }
        }
    }

    if (!FileManager::IsPathExists(fullPath))
    {
        MR_LOG(LogLocator, Fatal, "Directory does not exist! (%s)", sourceDirectory.Chr());
    }


    ListDirectory(fullPath, foundPaths);

    std::vector<String> a;
    SearchExtensionSpecified(fullPath, "mrbuild", a);
    
    for (String& path : a)
    {
        Module* t = Module::CreateModule(path);

        int h = 5;
    }

    return true;
}

void Locator::ListDirectory(const String& directory, std::vector<String>& returned)
{
    WIN32_FIND_DATAW find;

    wchar_t* path = Layer::GetSystemLayer()->ConvertToWide(String::Format("%s\\*", directory.Chr()));
    HANDLE file = FindFirstFileW(path, &find);

    if (file != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (find.cFileName[0] == L'.')
                continue;

            switch (find.dwFileAttributes)
            {
                case FILE_ATTRIBUTE_DIRECTORY:
                {
                    ListDirectory(String::Format("%s\\%ls", directory.Chr(), find.cFileName), returned);
                    break;
                }

                case FILE_ATTRIBUTE_ARCHIVE:
                {
                    returned.push_back(String::Format("%s\\%ls", directory.Chr(), find.cFileName));
                    break;
                }
            }

        } while (FindNextFileW(file, &find) != 0);
    }
    else
    {
        MR_LOG(LogLocator, Error, "Directory handle invalid!");
    }

    delete[] path;
}

void Locator::SearchExtensionSpecified(const String& directory, const String& extension, std::vector<String>& returned)
{
    WIN32_FIND_DATAW find;

    wchar_t* path = Layer::GetSystemLayer()->ConvertToWide(String::Format("%s\\*", directory.Chr()));
    HANDLE file = FindFirstFileW(path, &find);

    if (file != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (find.cFileName[0] == L'.')
                continue;

            switch (find.dwFileAttributes)
            {
                case FILE_ATTRIBUTE_DIRECTORY:
                {
                    SearchExtensionSpecified(String::Format("%s\\%ls", directory.Chr(), find.cFileName), extension, returned);
                    break;
                }

                case FILE_ATTRIBUTE_ARCHIVE:
                {
                    if (!FileManager::IsEndingWith(find.cFileName, extension))
                        break;

                    returned.push_back(String::Format("%s\\%ls", directory.Chr(), find.cFileName));
                    break;
                }
            }

        } while (FindNextFileW(file, &find) != 0);
    }
    else
    {
        MR_LOG(LogLocator, Error, "Directory handle invalid!");
    }

    delete[] path;
}

