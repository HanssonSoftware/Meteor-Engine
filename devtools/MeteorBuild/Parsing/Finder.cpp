/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Finder.h"
#include <Types/String.h>
#include <Paths.h>
#include <Windows/Windows.h>
#include <Platform/FileManager.h>
#include <Platform/Platform.h>
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

            ScopedPtr<wchar_t> wideExecutableDirectory = Platform::ConvertToWide(fullPath.Chr());

            PathCchRemoveFileSpec(wideExecutableDirectory.Get(), wcslen(wideExecutableDirectory.Get()));
            fullPath = String::Format("%ls\\%s", wideExecutableDirectory.Get(), fullDirectoryToAll.Chr());
            ScopedPtr<wchar_t> wideExecutableDirectoryNoFile = Platform::ConvertToWide(fullPath.Chr());

            wchar_t* canonicalizedPath = nullptr;
            if (PathAllocCanonicalize(wideExecutableDirectoryNoFile.Get(), PATHCCH_ALLOW_LONG_PATHS, &canonicalizedPath) != S_OK)
            {
                MR_LOG(LogLocator, Fatal, "PathAllocCanonicalize returned: %s", Platform::GetError().Chr());
            }

            fullPath = canonicalizedPath;

            LocalFree(canonicalizedPath);
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

    ScopedPtr<wchar_t> path = Platform::ConvertToWide(String::Format("%s\\*", directory.Chr()));
    HANDLE file = FindFirstFileW(path.Get(), &find);

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
}

void Finder::SearchExtensionSpecified(const String& directory, const String& extension, Array<String>& returned)
{
    WIN32_FIND_DATAW find;

    ScopedPtr<wchar_t> path = Platform::ConvertToWide(String::Format("%s\\*", directory.Chr()));
    HANDLE file = FindFirstFileW(path.Get(), &find);

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
}

bool Finder::FindMainScript(String& path)
{
    ScopedPtr<wchar_t> fullPath = nullptr;

    bool bIsPathWasRelative = false;
    if (FileManager::IsPathRelative(path))
    {
        ScopedPtr<wchar_t> exeDir = Platform::ConvertToWide(Paths::GetExecutableDirctory());
        ScopedPtr<wchar_t> convertedPath = Platform::ConvertToWide(path);

        PathCchRemoveFileSpec(exeDir.Get(), wcslen(exeDir.Get()));
        if (FAILED(PathAllocCombine(exeDir.Get(), convertedPath.Get(), PATHCCH_ALLOW_LONG_PATHS, &fullPath)))
        {
            MR_LOG(LogLocator, Fatal, "PathAllocCombine returned: %s", *Platform::GetError())
        }

        bIsPathWasRelative = true;
    }
    else
    {
        fullPath = Platform::ConvertToWide(path);
    }

    StringCchCatW(fullPath.Get(), wcslen(fullPath.Get()) + wcslen(L"\\*") + 1, L"\\*");

    WIN32_FIND_DATAW data;
    HANDLE firstFound = FindFirstFileW(fullPath.Get(), &data);

    if (firstFound != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (data.cFileName[0] == '.')
                continue;

            if (FileManager::IsEndingWith(data.cFileName, "mrbuild"))
            {
                fullPath.Get()[wcslen(fullPath.Get()) - 2] = L'\0';

                path = String::Format("%ls\\%ls", fullPath.Get(), data.cFileName);
                break;
            }


        } while (FindNextFileW(firstFound, &data));

        LocalFree(firstFound);
        return true;
    }
    else
    {
        MR_LOG(LogLocator, Fatal, "FindFirstFileW encountered an invalid handle! %s", *Platform::GetError());
    }

    return false;
}