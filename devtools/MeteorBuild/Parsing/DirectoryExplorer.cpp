/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "DirectoryExplorer.h"
#include <Log/LogMacros.h>
#include <Application/Application.h>
#include <pathcch.h>
#include <File/File.h>
#pragma comment(lib, "Pathcch.lib")

LOG_ADDCATEGORY(DirectoryExplorer);

DirectoryExplorer::DirectoryExplorer()
{
}

DirectoryExplorer::~DirectoryExplorer()
{
}


static bool bIsFirstCall = false;
void DirectoryExplorer::startExpedition(String directory)
{
    if (directory.isEmpty())
    {
        MR_LOG(LogDirectoryExplorer, Fatal, TEXT("directory is Empty! Consider Checking via Debugger!"));
    }

    static String directoryLastFolder;
    if (!bIsFirstCall)
    {
        bIsFirstCall = true;

        String formatted = Application::Get()->getApplicationDirectory();

        PathCchRemoveFileSpec(formatted.Data(), wcslen(formatted.Chr()));

        formatted = String::Format("%s\\%s", formatted.Chr(), directory.Chr());
        PathCchCanonicalize(directory.Data(), wcslen(formatted.Chr()) + 1, formatted.Chr());
        directoryLastFolder = directory;
    }

    WIN32_FIND_DATAW searchData;
    HANDLE file = FindFirstFile((directory + "\\*").Chr(), &searchData);
    if (file != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (searchData.cFileName[0] == L'.')
                continue;


            if (searchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                const String newDir = String::Format("%s\\%s", directory.Chr(), searchData.cFileName);
                MR_LOG(LogDirectoryExplorer, Verbose, TEXT("%s"), newDir.Chr());
                startExpedition(newDir);
            }
            else if (searchData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
            {
                const String FileNameWExtension = searchData.cFileName;
                if (FileNameWExtension.endsWith("mrbuild"))
                {
                    const String newModule = String::Format("%s %s", directory.Chr(), searchData.cFileName);
                    MR_LOG(LogDirectoryExplorer, Verbose, TEXT("%s"), newModule.Chr());
                    modules.push_back(newModule);
                }
            }

        } while (FindNextFile(file, &searchData) != 0);
    }
}

std::vector<FMFile*> DirectoryExplorer::processModules()
{
    std::vector<FMFile*> files;
    for (String temp : modules)
    {
        wchar_t* tempA = temp.Data();
        wchar_t* tempB;

        wcstok(tempA, L" ", &tempB);

        FMFile newFile;
        if (newFile.Open(String::Format("%s\\%s", tempA, tempB), OPENRULE_READ, OVERRIDERULE_JUST_OPEN) != FILESTATUS_GOOD)
        {
            MR_LOG(LogDirectoryExplorer, Error, TEXT("Unable to Open %s"), newFile.getName().Chr());
            continue;
        }

        newFile.Read();
        processToModuleDescriptor(newFile.getBuffer());

        files.push_back(&newFile);
    }

    return files;
}

static size_t bufferLength = 0;
inline String DirectoryExplorer::processThatLine(const wchar_t* buffer, int A, int B)
{
    if (!buffer)
        return String();

    if (bufferLength == 0)
        bufferLength = wcslen(buffer);

    if (A > bufferLength)
        A = bufferLength;

    if (B > bufferLength)
        B = bufferLength;

    std::vector<wchar_t> data;
    for (int i = A; i < B; i++)
    {

    }

    return String();
}

bool DirectoryExplorer::processToModuleDescriptor(const wchar_t* buffer)
{
    if (!buffer)
        return false;

    const wchar_t* ptr = buffer;

    int index = 0;
    while (buffer[index] != L'\n')
        index++;

    return false;
}

