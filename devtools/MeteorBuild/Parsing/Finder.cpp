/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Finder.h"
#include <Types/String.h>
#include <Paths.h>
#include <Windows/Windows.h>
#include <Platform/FileManager.h>
#include <Layers/SystemLayer.h>
#include <PathCch.h>
#include <strsafe.h>

#pragma comment(lib, "Pathcch.lib")

LOG_ADDCATEGORY(Locator);

static bool bIsFirstCall = true;

void Finder::LocateSources(const String& fullDirectoryToAll, Array<String>& returned)
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
}

void Finder::ListDirectory(const String& directory, Array<String>& returned)
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

void Finder::SearchExtensionSpecified(const String& directory, const String& extension, Array<String>& returned)
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

bool Finder::FindMainScript(String& path)
{
    wchar_t* fullPath = nullptr;

    bool bIsPathWasRelative = false;
    if (FileManager::IsPathRelative(path))
    {
        wchar_t* exeDir = Layer::GetSystemLayer()->ConvertToWide(Paths::GetExecutableDirctory());
        wchar_t* convertedPath = Layer::GetSystemLayer()->ConvertToWide(path);

        PathCchRemoveFileSpec(exeDir, wcslen(exeDir));
        if (FAILED(PathAllocCombine(exeDir, convertedPath, PATHCCH_ALLOW_LONG_PATHS, &fullPath)))
        {
            MR_LOG(LogLocator, Fatal, "PathAllocCombine returned: %s", *Layer::GetSystemLayer()->GetError())
        }

        bIsPathWasRelative = true;
        delete[] exeDir, convertedPath;
    }
    else
    {
        fullPath = Layer::GetSystemLayer()->ConvertToWide(path);
    }

    StringCchCatW(fullPath, wcslen(fullPath) + wcslen(L"\\*") + 1, L"\\*");

    WIN32_FIND_DATAW data;
    HANDLE firstFound = FindFirstFileW(fullPath, &data);

    if (firstFound != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (data.cFileName[0] == '.')
                continue;

            if (FileManager::IsEndingWith(data.cFileName, "mrbuild"))
            {
                fullPath[wcslen(fullPath) - 2] = L'\0';

                path = String::Format("%ls\\%ls", fullPath, data.cFileName);
                break;
            }


        } while (FindNextFileW(firstFound, &data));

        if (bIsPathWasRelative)
            LocalFree(fullPath);
        else
            delete[] fullPath;

        LocalFree(firstFound);
        return true;
    }
    else
    {
        MR_LOG(LogLocator, Fatal, "FindFirstFileW encountered an invalid handle! %s", *Layer::GetSystemLayer()->GetError());
    }

    if (bIsPathWasRelative)
    LocalFree(fullPath);
    else
    delete[] fullPath;

    return false;
}