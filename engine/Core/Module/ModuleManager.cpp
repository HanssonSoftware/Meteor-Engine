/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "ModuleManager.h"
#include <PlatformLayout.h>

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/Windows.h>
#endif // MR_PLATFORM_WINDOWS

bool ModuleManager::LoadModule(const String& moduleName)
{
#ifdef MR_PLATFORM_WINDOWS
    

    //LoadLibraryW();
#endif // MR_PLATFORM_WINDOWS



    return false;
}
