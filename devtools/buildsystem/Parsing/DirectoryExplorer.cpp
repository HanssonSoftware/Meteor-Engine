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

inline void DirectoryExplorer::goBack(String& input)
{
    PathCchRemoveFileSpec(input.Data(), wcslen(input.Chr()));
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

        PathCchRemoveFileSpec(formatted.Data(), wcslen(formatted.Chr()) + 1);

        formatted = String::Format("%s\\%s", formatted, directory.Chr());
        PathCchCanonicalize(directory.Data(), wcslen(formatted.Chr()) + 1, formatted.Chr());
        directoryLastFolder = directory;
    }
}

