/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "LocatingInterface.h"
#include <Types/String.h>
#include <Paths.h>
#include <Windows/Windows.h>
#include <Platform/FileManager.h>
#include <Layers/SystemLayer.h>
#include <PathCch.h>

#pragma comment(lib, "Pathcch.lib")

LOG_ADDCATEGORY(Locator);

bool Locator::FindAllReferences(const String& sourceDirectory)
{
    MR_ASSERT(!(!sourceDirectory), "Directory is empty! Consider checking via Debugger!");
    


    //if (!FileManager::IsPathExists(fullPath))
    {
        MR_LOG(LogLocator, Fatal, "Directory does not exist! (%s)", sourceDirectory.Chr());
    }

    Array<String> foundScripts;
    //SearchExtensionSpecified(fullPath, "mrbuild", foundScripts);
    
    for (String& path : foundScripts)
    {
        Module* t = Module::CreateModule(path);

        int h = 5;
    }

    return true;
}

static bool bIsFirstCall = true;

void Locator::LocateSources(const String& fullDirectoryToAll, Array<String>& returned)
{
    String fullPath = fullDirectoryToAll;
    if (bIsFirstCall)
    {
        bIsFirstCall = false;

        if (FileManager::IsPathRelative(fullDirectoryToAll))
        {
            fullPath = Paths::GetExecutableDirctory();
            if (SystemLayer* systemLayer = Layer::GetSystemLayer())
            {
                wchar_t* wideExecutableDirectory = systemLayer->ConvertToWide(fullPath.Chr());

                PathCchRemoveFileSpec(wideExecutableDirectory, wcslen(wideExecutableDirectory));
                fullPath = String::Format("%ls\\%s", wideExecutableDirectory, fullDirectoryToAll.Chr());
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
            if (!FileManager::IsPathExists(fullDirectoryToAll))
            {
                MR_LOG(LogLocator, Fatal, "Directory does not exist! (%s)", fullDirectoryToAll.Chr());
            }
        }
    }

    ListDirectory(fullPath, returned);

    //for (String& temp : returned)
    //{
    //    OutputDebugStringA(temp.Chr());
    //    OutputDebugStringA("\n");
    //}
}

void Locator::ListDirectory(const String& directory, Array<String>& returned)
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
                    returned.Add(String::Format("%s\\%ls", directory.Chr(), find.cFileName));
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

void Locator::SearchExtensionSpecified(const String& directory, const String& extension, Array<String>& returned)
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

                    returned.Add(String::Format("%s\\%ls", directory.Chr(), find.cFileName));
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

