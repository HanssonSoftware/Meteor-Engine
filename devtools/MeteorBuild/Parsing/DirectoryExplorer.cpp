/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "DirectoryExplorer.h"
#include <Logging/LogMacros.h>
#include <Application.h>
#include <pathcch.h>
#include <Generic/File.h>
#include <Windows/WinAPI.h>
#include <Layers/OSLayer.h>

#pragma comment(lib, "Pathcch.lib")
#pragma warning(disable : 6031)
#pragma warning(disable : 6001) // Using uninitialized memory 'tempB'.
 
LOG_ADDCATEGORY(DirectoryExplorer);

DirectoryExplorer::DirectoryExplorer()
{
}

DirectoryExplorer::~DirectoryExplorer()
{
}


static bool bIsFirstCall = false;
void DirectoryExplorer::StartExpedition(String directory)
{
    if (directory.IsEmpty())
    {
        MR_LOG(LogDirectoryExplorer, Fatal, TEXT("directory is Empty! Consider Checking via Debugger!"));
    }

    static String directoryLastFolder;
    if (!bIsFirstCall)
    {
        bIsFirstCall = true;

        String formatted = Application::Get()->GetApplicationDirectory();

        if (OSLayer* layer = OSLayer::GetSystemLayer())
        {
            wchar_t* text = layer->ConvertToWide(formatted.Data());

            PathCchRemoveFileSpec(text, wcslen(text));

            formatted = String::Format("%s\\%s", formatted.Chr(), directory.Chr());
            PathCchCanonicalize(directory.Data(), wcslen(formatted.Chr()) + 1, formatted.Chr());
            directoryLastFolder = directory;

        }
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
                StartExpedition(newDir);
            }
            else if (searchData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
            {
                const String FileNameWExtension = searchData.cFileName;
                if (FileNameWExtension.EndsWith("mrbuild"))
                {
                    const String newModule = String::Format("%s\\%s", directory.Chr(), searchData.cFileName);
                    MR_LOG(LogDirectoryExplorer, Verbose, TEXT("%s"), newModule.Chr());
                    modules.push_back(newModule);
                }
            }

        } while (FindNextFile(file, &searchData) != 0);
    }
}

std::vector<IFile*> DirectoryExplorer::processModules()
{
    std::vector<IFile*> files(modules.size());
    for (String temp : modules)
    {
        IFile* newFile = CreateFileOperation();
        if (newFile->Open(temp, OPENRULE_READ, OVERRIDERULE_JUST_OPEN) != FILESTATUS_GOOD)
        {
            MR_LOG(LogDirectoryExplorer, Error, TEXT("Unable to Open %s"), *newFile->GetName());
            continue;
        }

        newFile->Read();
        ProcessToModuleDescriptor(newFile->GetBuffer());

        files.push_back(newFile);
    }

    return files;
}

bool DirectoryExplorer::ProcessToModuleDescriptor(const char* buffer)
{
    int sdfjk = 5;
    String p = String::ReadLine(buffer, sdfjk);

    return false;
}