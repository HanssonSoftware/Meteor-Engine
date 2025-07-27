/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Application.h"
#include <Platform/WindowManager/WindowManager.h>
#include <Platform/FileManager.h>

#include <MemoryManager.h>
#include <Layers/SystemLayer.h>
#include <Layers/LayerManager.h>
//#include <GraphicsEngine/SceneGraph.h>
#include <mutex>
#include "Commandlet.h"
#ifdef MR_DEBUG
#include <crtdbg.h>
#endif // MR_DEBUG
#include <Platform/PerformanceTimer.h>
#include <Renderer/Registry.h>

static Application* appFramework = nullptr;

Application::Application()
{
#ifdef _WIN64
    WindowsWindowManager* wm = new WindowsWindowManager();
#else

#endif // _WIN64
    LayerManager* lm = new LayerManager();

    SetWindowManager(wm);
    SetLayerManager(lm);
}

Application* Application::Get()
{
    return appFramework;
}

void Application::RequestExit(int Code)
{
    MR_ASSERT(appFramework != nullptr, "Application class is invalid!");
    
    appFramework->exitCode = Code;
    appFramework->SetAppState(APPLICATIONSTATE_SHUTDOWN);
}

const int Application::GetRequestExitCode()
{
    MR_ASSERT(appFramework != nullptr, "Application class is invalid!");

    return appFramework->exitCode;
}

void Application::Init()
{
#ifdef MR_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif // MR_DEBUG

    layerManager->Init();
    MemoryManager::Initialize(appFramework->appInfo.requiredMinimumMemoryInPercent);
    Logger::Initialize();

    if (Application::Get()->appInfo.flags &~ APPFLAG_NO_WINDOW)
        windowManager->Init();


    MR_LOG(LogApplication, Log, "Initializing application.");

    if (appInfo.appName.IsEmpty())
    {
        MR_LOG(LogApplication, Fatal, "AppInfo is invalid!");
    }

    CreateNativeWindow();
    
    if (Application::Get()->appInfo.flags & APPFLAG_DISPLAY_QUICK_INFO_ABOUT_MEMORY_USAGE)
        int h = 5; // DELETE

    SetAppState(APPLICATIONSTATE_RUNNING);
}

bool Application::InstantiateApplication(Application* newInstance, const ApplicationInitializationInfo* appCreateInfo)
{
    if (!newInstance || !appCreateInfo)
    {
        Application::RequestExit(-1);
        return false;
    }

    appFramework = newInstance;
    appFramework->appInfo = *appCreateInfo;

    appFramework->Init();

    return true;
}

void Application::Run()
{
    VulkanRegistry* reg = VulkanRegistry::GetRegistry();
    SetAppState(APPLICATIONSTATE_RUNNING);

    while (GetAppState() == APPLICATIONSTATE_RUNNING)
    {
        layerManager->UpdateLayer();
        appFramework->Run();
        reg->Render();
    }

    appFramework->Shutdown();
}

void Application::Shutdown()
{
    if (GetAppState() == APPLICATIONSTATE_RESTARTING)
    {
        MR_LOG(LogApplication, Log, "Restarting application!");
    
        windowManager->Shutdown();

        layerManager->Shutdown();

        Logger::Shutdown();

        appFramework->Init();
    }
    else
    {
        MR_LOG(LogApplication, Log, "Shutting down application!");

        windowManager->Shutdown();

        layerManager->Shutdown();
        
        Logger::Shutdown();

        MemoryManager::Shutdown();
    }
}

String Application::GetApplicationDirectory()
{
    return String();
}

Application::~Application()
{
    if (windowManager)
        windowManager = nullptr;
    
    if (layerManager)
        layerManager = nullptr;
}

String Application::GetApplicationName()
{
    if (appFramework->appInfo.appName.IsEmpty())
    {
        return appFramework->GetWindowManager()->GetFirstWindow()->windowData.windowName;
    }

    return appFramework->appInfo.appName;
}


void Application::CreateNativeWindow() const
{
    if (appFramework->appInfo.flags & APPFLAG_NO_WINDOW)
        return;

    GetWindowManager()->CreateNativeWindow(&appFramework->appInfo.windowCreateInfo);
}

Application* GetApplication()
{
    return appFramework;
}
