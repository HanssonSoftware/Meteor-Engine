/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "DirectoryExplorer.h"
#include <Log/LogMacros.h>
#include <Application/Application.h>
#include <pathcch.h>
#include <filesystem>

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

    MR_LOG(LogDirectoryExplorer, Log, TEXT("Directory Search Begin! %s"), directory.Chr());

    static String directoryLastFolder;
    if (!bIsFirstCall)
    {
        bIsFirstCall = true;

        // E:\\meteor\\product\\MeteorBuild.exe
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
                startExpedition(String::Format("%s\\%s", directory.Chr(), searchData.cFileName));
            }
            else if (searchData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
            {
                const String FileNameWExtension = searchData.cFileName;
                if (FileNameWExtension.endsWith("mrbuild"))
                {
                    modules.push_back(String::Format("%s %s", directory.Chr(), searchData.cFileName));
                    //MR_LOG(LogDirectoryExplorer, Log, TEXT("NewFile! %s"), searchData.cFileName);
                }
            }

        } while (FindNextFile(file, &searchData) != 0);
    }
}

void DirectoryExplorer::processModules()
{

}

