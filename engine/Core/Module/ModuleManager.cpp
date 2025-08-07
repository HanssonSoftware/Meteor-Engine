/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "ModuleManager.h"
#include <Layers/SystemLayer.h>
#include <PlatformLayout.h>

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/Windows.h>
#endif // MR_PLATFORM_WINDOWS

#pragma warning(disable : 6387)

static constexpr const char* engineName = "MeteorEngine";

LOG_ADDCATEGORY(ModuleManager);

ModuleManager& ModuleManager::Get()
{
    static ModuleManager instance;
    return instance;
}

ModuleManager::~ModuleManager() noexcept
{

}

bool ModuleManager::LoadModule(const String& moduleName)
{
    if (SystemLayer* layer = Layer::GetSystemLayer())
    {

#ifdef MR_PLATFORM_WINDOWS
        wchar_t* libraryName = layer->ConvertToWide(String::Format("%s-%s.dll", engineName, moduleName.Chr()));

        HMODULE module = LoadLibraryW(libraryName);


        if (!FreeLibrary(module))
        {
            MR_LOG(LogModuleManager, Error, "FreeLibrary returned: %s", *layer->GetError());
            return false;
        }
#endif // MR_PLATFORM_WINDOWS
        return true;
    }

    return false;
}
