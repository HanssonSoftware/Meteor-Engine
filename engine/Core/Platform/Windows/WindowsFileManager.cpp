/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS

#include "WindowsFileManager.h"
#include <Logging/Log.h>
#include <Layers/Layer.h>
#include <Application.h>
#include <Layers/SystemLayer.h>
#include <Windows/WindowsPaths.h>
#include <Windows/WindowsFile.h>
#include <Platform/Platform.h>

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

    wchar_t* dirName = Layer::GetSystemLayer()->ConvertToWide(name.Chr());
    if (!dirName)
    {
        MR_LOG(LogFileManager, Error, "Invalid wide buffer!");
        return false;
    }

    if (IsPathRelative(name))
    {
        wchar_t* dirAbsoluteConvert = Layer::GetSystemLayer()->ConvertToWide(Paths::GetExecutableDirctory().Chr());
        PathCchRemoveFileSpec(dirAbsoluteConvert, wcslen(dirAbsoluteConvert));

        wchar_t* combined = nullptr;
        if (FAILED(PathAllocCombine(dirAbsoluteConvert, dirName, PATHCCH_ALLOW_LONG_PATHS, &combined)))
        {
            MR_LOG(LogFileManager, Error, "Failed to convert to full path: %s", *Layer::GetSystemLayer()->GetError());
            return false;
        }

        delete[] dirAbsoluteConvert, dirName;
        dirName = combined;
    }

    for (wchar_t* p = dirName; *p; ++p)
    {
        if (*p == L'/')
            *p = L'\\';
    }

    const int32_t result = SHCreateDirectoryExW(nullptr, dirName, nullptr);
    if (result != ERROR_SUCCESS && result != ERROR_ALREADY_EXISTS)
    {
        if (PathIsRelativeW(dirName))
            delete[] dirName;
        else
            LocalFree(dirName);

        MR_LOG(LogFileManager, Error, "SHCreateDirectory returned: %s", *Layer::GetSystemLayer()->GetError());
        
        return false;
    }

    if (!PathIsRelativeW(dirName))
        LocalFree(dirName);
    else
        delete[] dirName;

    return true;
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
        delete[] dirName;
        MR_LOG(LogFileManager, Error, "RemoveDirectoryW returned: %s", *Layer::GetSystemLayer()->GetError());
        
        return false;
    }

    delete[] dirName;
    return true;
} 

bool WindowsFileManager::IsPathExists(const String& name)
{
    ScopedPtr<wchar_t> dirName = Platform::ConvertToWide(name.Chr());

    if (!dirName.Get())
    {
        MR_LOG(LogFileManager, Error, "Invalid wide buffer!");
        return false;
    }

    if (!::PathFileExistsW(dirName.Get()))
    {
        if (GetLastError() != ERROR_FILE_NOT_FOUND)
        {
            MR_LOG(LogFileManager, Error, "IsPathExists returned: %s", Platform::GetError());
        }

        return false;
    }
   
    return true;
}

/** Returns true if the path is qualified, or false otherwise. */
bool WindowsFileManager::IsPathRelative(const String& path)
{
    ScopedPtr<wchar_t> buffer = Platform::ConvertToWide(path.Chr());

    return PathIsRelativeW(buffer.Get()) ? true : false;
}

bool WindowsFileManager::IsEndingWith(const String& name, const String& extension)
{
    if (name.IsEmpty() && extension.IsEmpty())
        return false;

    if (SystemLayer* systemLayer = Layer::GetSystemLayer())
    {
        wchar_t* buffer = systemLayer->ConvertToWide(name.Chr());

        LPWSTR result = PathFindExtensionW(buffer);
        if (wcslen(result) == 0)
            return false;

        memmove(result, result + 1, (wcslen(result) + 1) * sizeof(wchar_t));

        bool bIsGood = false;
        wchar_t* extensionConverted = systemLayer->ConvertToWide(extension.Chr());

        bIsGood = wcscmp(result, extensionConverted) == 0 ? true : false;
        delete[] buffer, extensionConverted;

        return bIsGood ? true : false;
    }

    return false;
}

void WindowsFileManager::NormalizeDirectory(String& input)
{
    char* buffer = input.Allocate();
    uint32_t size = (int32_t)input.Length();

    for (uint32_t i = 0; i < size; i++)
    {
        if (buffer[i] == '/' || buffer[i] == '//')
        {
            buffer[i] = '\\';
        }
    }

    size = (uint32_t)strlen(buffer);
    buffer[size] = '\0';

    input = String(buffer);

    delete[] buffer;
}

IFile* WindowsFileManager::CreateFileOperation(const String& pathA, int32_t accessType, int32_t sharingMode, FileOverrideRules createType, FileStatus& status)
{
    String path = pathA;
    NormalizeDirectory(path);

    ScopedPtr<wchar_t> name = Platform::ConvertToWide(path.Chr());

    if (!IsPathExists(path))
        FileManager::CreateDirectory(path, true);

    HANDLE file = CreateFileW(
        name.Get(),
        evaluateAccessTypeForCreateFileOperation(accessType),
        evaluateSharingModeForCreateFileOperation(sharingMode),
        nullptr,
        evaluateCreateTypeForCreateFileOperation(createType),
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file == INVALID_HANDLE_VALUE)
    {
        MR_LOG(LogFileManager, Error, "%s", *Platform::GetError());
        return nullptr;
    }

    WindowsFile* newWindowsFile = new WindowsFile();
    newWindowsFile->fileHandle = file;
    newWindowsFile->fileName = name.Get();

    return newWindowsFile;
}

constexpr const int32_t WindowsFileManager::evaluateAccessTypeForCreateFileOperation(int32_t accessType)
{
    int32_t accessTypeFast = 0;

    if (accessType & OPENMODE_NONE) accessTypeFast |= 0;
    if (accessType & OPENMODE_READ) accessTypeFast |= GENERIC_READ;
    if (accessType & OPENMODE_WRITE) accessTypeFast |= GENERIC_WRITE;

    return accessTypeFast;
}

constexpr const int32_t WindowsFileManager::evaluateSharingModeForCreateFileOperation(int32_t sharingMode)
{
    int32_t sharingModeFast = 0;

    if (sharingMode & SHAREMODE_NONE) sharingModeFast |= 0;
    if (sharingMode & SHAREMODE_READ) sharingModeFast |= FILE_SHARE_READ;
    if (sharingMode & SHAREMODE_WRITE) sharingModeFast |= FILE_SHARE_WRITE;
    if (sharingMode & SHAREMODE_DELETE) sharingModeFast |= FILE_SHARE_DELETE;

    return sharingModeFast;
}

constexpr const int32_t WindowsFileManager::evaluateCreateTypeForCreateFileOperation(FileOverrideRules createType)
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
    const uint32_t dirSize = (uint32_t)wcslen(dir);

    if (PathCchRemoveExtension(dir, dirSize) == S_OK)
        /*PathCchRemoveExtension*/ PathCchRemoveFileSpec(dir, dirSize);

    DWORD requiredAmount = GetCurrentDirectoryW(0, 0);
    wchar_t* super = new wchar_t[requiredAmount + 1]();
    GetCurrentDirectoryW(requiredAmount, super);

    wchar_t* superSuper;
    HRESULT result = PathAllocCombine(super, dir, PATHCCH_ALLOW_LONG_PATHS | PATHCCH_FORCE_ENABLE_LONG_NAME_PROCESS, &superSuper);

    int32_t resultA = SHCreateDirectory(0, superSuper);
    
    delete[] super;
    LocalFree(superSuper);
    
    return true;
}