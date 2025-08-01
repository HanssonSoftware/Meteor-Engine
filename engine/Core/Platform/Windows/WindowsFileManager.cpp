/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS

#include "WindowsFileManager.h"
#include <Types/String.h>
#include <Logging/LogMacros.h>
#include <Layers/Layer.h>
#include <Application.h>
#include <Layers/SystemLayer.h>
#include <Windows/WindowsFile.h>

#include <Windows/Windows.h>
#undef CreateDirectory

#include <shlwapi.h>
#include <PathCch.h>
#include <Shlobj.h>

#pragma comment (lib, "Shlwapi.lib")
#pragma comment (lib, "Pathcch.lib")
#pragma comment (lib, "Shell32.lib")


bool WindowsFileManager::CreateDirectory(const String& name, bool bToFullPath)
{
    MR_ASSERT(Layer::GetSystemLayer() != nullptr, "System Layer does not initialized!");

    if (name.IsEmpty())
    {
        MR_LOG(LogFileManager, Error, "Invalid directory name!");
        return false;
    }

    String tm = name;
    NormalizeDirectory(tm);

    wchar_t* dirName = Layer::GetSystemLayer()->ConvertToWide(tm.Chr());
    if (!dirName)
    {
        MR_LOG(LogFileManager, Error, "Invalid wide buffer!");
        return false;
    }

    startRecursiveCreate(dirName);

    delete[] dirName;
    return false;
}

bool WindowsFileManager::DeleteDirectory(const String& name, bool bToFullPath)
{
    MR_ASSERT(Layer::GetSystemLayer() != nullptr, "System Layer does not initialized!");

    wchar_t* dirName = Layer::GetSystemLayer()->ConvertToWide(name.Chr());
    if (!dirName)
    {
        MR_LOG(LogFileManager, Error, "Invalid wide buffer!");
        return false;
    }

    if (!RemoveDirectoryW(dirName))
    {
        MR_LOG(LogFileManager, Error, "Error: %s", Layer::GetSystemLayer()->GetError());
        delete[] dirName;
        return false;
    }

    delete[] dirName;
    return true;
}

void WindowsFileManager::ListDirectory(String dir, std::vector<String>& output)
{
    if (FileManager::IsPathRelative(dir))
    {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(nullptr, path, MAX_PATH); 
        size_t written = wcslen(path);

        PathCchRemoveFileSpec(path, written);

        wchar_t* bf = Layer::GetSystemLayer()->ConvertToWide(dir);
        PathCchAppend(path, written + dir.Length(), bf);

        delete[] bf;

        dir = path;
    }


    WIN32_FIND_DATAW find;

    wchar_t* path = Layer::GetSystemLayer()->ConvertToWide(String::Format("%s\\*", dir.Chr()));
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
                    ListDirectory(String::Format("%s\\%ls", dir.Chr(), find.cFileName), output);
                    break;
                }

                case FILE_ATTRIBUTE_ARCHIVE:
                {
                    output.push_back(String::Format("%s\\%ls", dir.Chr(), find.cFileName));
                    break;
                }
            }

        } while (FindNextFileW(file, &find) != 0);
    }
    else
    {
        MR_LOG(LogFileManager, Error, "Directory handle invalid!");
    }

    delete[] path;
}

bool WindowsFileManager::IsPathExists(const String& name)
{
    MR_ASSERT(Layer::GetSystemLayer() != nullptr, "System Layer does not initialized!");

    if (name.IsEmpty())
    {
        MR_LOG(LogFileManager, Error, "Invalid directory name!");
        return false;
    }

    if (SystemLayer* systemLayer = Layer::GetSystemLayer())
    {
        wchar_t* dirName = systemLayer->ConvertToWide(name.Chr());

        if (!dirName)
        {
            MR_LOG(LogFileManager, Error, "Invalid wide buffer!");

            delete[] dirName;
            return false;
        }

        if (::PathFileExistsW(dirName) == 0)
        {
            if (GetLastError() != ERROR_FILE_NOT_FOUND)
            {
                MR_LOG(LogFileManager, Error, "IsPathExists returned: %s", Layer::GetSystemLayer()->GetError().Chr());
            }

            delete[] dirName;
            return false;
        }

        delete[] dirName;
        return true;
    }

   
    return false;
}

/** Returns true if the path is qualified, or false otherwise. */
bool WindowsFileManager::IsPathRelative(const String& path)
{
    MR_ASSERT(Layer::GetSystemLayer() != nullptr, "System Layer does not initialized!");

    if (SystemLayer* systemLayer = Layer::GetSystemLayer())
    {
        wchar_t* buffer = systemLayer->ConvertToWide(path.Chr());

        const BOOL result = PathIsRelativeW(buffer);
        delete[] buffer;

        return result ? true : false;
    }

    return false;
}

bool WindowsFileManager::IsEndingWith(const String& name, const String& extension)
{
    return IFileManager::IsEndingWith(name, extension);
}

void WindowsFileManager::NormalizeDirectory(String& input)
{
    char* buffer = input.Allocate();
    memcpy(buffer, input.Chr(), input.Length());

    uint32 size = (int)input.Length();

    for (int i = 0; i < size; i++)
    {
        if (buffer[i] == '/' || buffer[i] == '//')
        {
            buffer[i] = '\\';
        }
    }

    size = (uint32)strlen(buffer);
    buffer[size] = '\0';

    input = String(buffer);

    delete[] buffer;
}

IFile* WindowsFileManager::CreateFileOperation(const String& pathA, int accessType, int sharingMode, FileOverrideRules createType, FileStatus& status)
{
    MR_ASSERT(Layer::GetSystemLayer(), "System Layer does not initialized!");

    String path = pathA;
    NormalizeDirectory(path);

    SystemLayer* systemLayer = Layer::GetSystemLayer();
    wchar_t* name = systemLayer->ConvertToWide(path.Chr());

    if (!IsPathExists(path))
    {
        FileManager::CreateDirectory(path, true);
    }

    HANDLE file = CreateFileW(
        name,
        evaluateAccessTypeForCreateFileOperation(accessType),
        evaluateSharingModeForCreateFileOperation(sharingMode),
        nullptr,
        evaluateCreateTypeForCreateFileOperation(createType),
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file == INVALID_HANDLE_VALUE)
    {
        MR_LOG(LogFileManager, Error, "%s", systemLayer->GetError());
        return nullptr;
    }

    WindowsFile* newWindowsFile = new WindowsFile();
    newWindowsFile->fileHandle = file;
    newWindowsFile->fileName = name;

    return newWindowsFile;
}

constexpr const int WindowsFileManager::evaluateAccessTypeForCreateFileOperation(int accessType)
{
    int accessTypeFast = 0;

    if (accessType & OPENMODE_NONE) accessTypeFast |= 0;
    if (accessType & OPENMODE_READ) accessTypeFast |= GENERIC_READ;
    if (accessType & OPENMODE_WRITE) accessTypeFast |= GENERIC_WRITE;

    return accessTypeFast;
}

constexpr const int WindowsFileManager::evaluateSharingModeForCreateFileOperation(int sharingMode)
{
    int sharingModeFast = 0;

    if (sharingMode & SHAREMODE_NONE) sharingModeFast |= 0;
    if (sharingMode & SHAREMODE_READ) sharingModeFast |= FILE_SHARE_READ;
    if (sharingMode & SHAREMODE_WRITE) sharingModeFast |= FILE_SHARE_WRITE;
    if (sharingMode & SHAREMODE_DELETE) sharingModeFast |= FILE_SHARE_DELETE;

    return sharingModeFast;
}

constexpr const int WindowsFileManager::evaluateCreateTypeForCreateFileOperation(FileOverrideRules createType)
{
    switch (createType)
    {
    case OVERRIDERULE_OPEN_ALWAYS:
        return 4;
    case OVERRIDERULE_OPEN_ONLY_IF_EXISTS:
        return 3;
    case OVERRIDERULE_CREATE_NEW_IF_NOT_EXISTS:
        return 1;
    case OVERRIDERULE_CREATE_NEW_DONT_MIND:
        return 2;
    default:
        return 0;
    }

    return 0;
}

// C:\\X\\Y\\FileName.txt
// ?\\FileName.txt

bool WindowsFileManager::startRecursiveCreate(wchar_t* dir)
{
    const size_t dirSize = wcslen(dir);

    if (PathCchRemoveExtension(dir, dirSize) == S_OK)
    {
        /*PathCchRemoveExtension*/ PathCchRemoveFileSpec(dir, dirSize);
    }

    DWORD requiredAmount = GetCurrentDirectoryW(0, 0);
    wchar_t* super = new wchar_t[requiredAmount + 1];
    GetCurrentDirectoryW(requiredAmount, super);

    wchar_t* superSuper;
    HRESULT result = PathAllocCombine(super, dir, PATHCCH_ALLOW_LONG_PATHS | PATHCCH_FORCE_ENABLE_LONG_NAME_PROCESS, &superSuper);

    int resultA = SHCreateDirectory(0, superSuper);
    
    delete[] super;
    LocalFree(superSuper);
    
    return true;
}


String WindowsFileManager::GetExecutableDirectory()
{
    static String location;

    if (location.IsEmpty())
    {
        wchar_t buf[MAX_PATH];
        DWORD count = GetModuleFileNameW(0, buf, MAX_PATH);

        location = buf;
    }

    return location;
}