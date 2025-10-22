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
#include <Module/ModuleManager.h>
#include <Renderer/Renderer.h>

Application::Application()
{
    appFramework = this;

#ifdef _WIN64
    auto wm = new WindowsWindowManager();
#else

#endif // _WIN64

    SetWindowManager(wm);
    SetLayerManager(new LayerManager());
}

Application* Application::Get()
{
    return appFramework;
}

void Application::RequestExit(int32_t Code)
{
    appFramework->exitCode = Code;
    appFramework->SetAppState(ECurrentApplicationState::SHUTDOWN);
}

void Application::Init()
{
#ifdef MR_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif // MR_DEBUG

    MemoryManager::Get().Initialize(appFramework->Memory.memoryReservePercent);
    
    windowManager->Init();
    ILogger::Get()->Initialize();

    MR_LOG(LogApplication, Log, "Initializing application.%s", "h");

    if (windowManager->CreateWindow(appName, { WindowData.x, WindowData.y }, true))
    {
        //Renderer::InitModule("Vulkan");
    }


    SetAppState(ECurrentApplicationState::RUNNING);
}

void Application::Run()
{
    while (GetAppState() == ECurrentApplicationState::RUNNING)
    {
        layerManager->UpdateLayer();
        appFramework->Run();
    }
}

void Application::Shutdown()
{
    if (GetAppState() == ECurrentApplicationState::RESTARTING)
    {
        MR_LOG(LogApplication, Log, "Restarting application!");

        windowManager->Shutdown();

        layerManager->Shutdown();

        ILogger::Get()->Shutdown();

        appFramework->Init();
    }
    else
    {
        MR_LOG(LogApplication, Log, "Shutting down application!");

        windowManager->Shutdown();

        layerManager->Shutdown();

        ILogger::Get()->Shutdown();

        MemoryManager::Get().Shutdown();
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
    //if (appFramework->appInfo.appName.IsEmpty())
    //{
    //    return appFramework->GetWindowManager()->GetFirstWindow()->windowData.windowName;
    //}

    return "appFramework->appInfo.appName";
}

Application* GetApplication()
{
    return Application::Get();
}
