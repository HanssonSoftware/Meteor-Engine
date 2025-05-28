/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Application.h"
#include <Platform/WindowManager/WindowManager.h>
#include <RHI/RHIOutputContext.h>
#include <RHI/RHIRegistry.h>
#include <thread>
#include <Platform/FileManager.h>

#include <Common/MemoryManager.h>
#include <Widgets/Viewport.h>
#include <Layers/OSLayer.h>
#include <Layers/LayerManager.h>
//#include <GraphicsEngine/SceneGraph.h>
#include <mutex>
#include "Commandlet.h"
#ifdef MR_DEBUG
#include <crtdbg.h>
#endif // MR_DEBUG
#include <Platform/PerformanceTimer.h>

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
    appFramework->Shutdown();
}

const int Application::GetRequestExitCode()
{
    MR_ASSERT(App::Get() != nullptr, "Application class is invalid!");

    return App::Get()->exitCode;
}

void Application::Init()
{
#ifdef MR_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif // MR_DEBUG

    windowManager->Init();
    layerManager->Init();

    Logger::Initialize();
    MR_LOG(LogApplication, Log, "Initializing application.");

    if (appInfo.appName.IsEmpty())
    {
        MR_LOG(LogApplication, Fatal, "AppInfo is invalid!");
    }

    CreateNativeWindow();
    
    if (Application::Get()->appInfo.flags & APPFLAG_DISPLAY_QUICK_INFO_ABOUT_MEMORY_USAGE)
        MemoryManager::Get().bQuickMemoryLogging = true;

    if (GetRenderContext())
    {
        //SceneGraph::Get().addToRoot(&nc);
        //GetRenderContext()->graphToRender = &SceneGraph::Get();
        //GetRenderContext()->setImGUIUsed(true);
    }

    SetAppState(APPLICATIONSTATE_RUNNING);
}

bool Application::InstantiateApplication(Application* newInstance, const ApplicationInitializationInfo* appCreateInfo)
{
    if (!newInstance || !appCreateInfo)
    {
        App::RequestExit(-1);
        return false;
    }

    appFramework = newInstance;
    appFramework->appInfo = *appCreateInfo;

    newInstance->Init();

    return true;
}

void Application::Run()
{
    IRHIRegistry* graphicsDevice = (IRHIRegistry*)GetWindowManager()->GetRenderContext();
  /*  if (GetWindowManager()->GetRenderContext() != nullptr)
        graphicsDevice->renderThread = std::thread([&graphicsDevice]() 
            { 
                while (Application::Get()->GetAppState() == ApplicationState::Running)
                {
                    if (graphicsDevice->getDeviceReadyState() == GRAPHICS_ENGINE_STATE_RUNNING)
                    {
                        graphicsDevice->Render();
                    }
                    else
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    }
                }
            }
        );*/


    SetAppState(APPLICATIONSTATE_RUNNING);

    while (GetAppState() == APPLICATIONSTATE_RUNNING)
    {
        PerformanceTimer tm;
        tm.Start();

        //MR_ASSERT();

	    if (graphicsDevice) graphicsDevice->GetOutputContext()->Clear();
		if (graphicsDevice) graphicsDevice->GetOutputContext()->Draw();

        tm.Stop();

        double t = tm.Evaluate();
        if (layerManager) layerManager->UpdateLayer();

        //SceneGraph::Get().Update(0.01f);
        appFramework->Run();
    }

    //if (windowManager->GetRenderContext() != nullptr)
    //{
    //    if (graphicsDevice->renderThread.joinable())
    //        graphicsDevice->renderThread.join();
    //}


    //windowManager->DestroyWindow("Super");
    appFramework->Shutdown();
}

void Application::Shutdown()
{
    if (GetAppState() == APPLICATIONSTATE_RESTARTING)
    {
        MR_LOG(LogApplication, Log, "Restarting Application!");
    
        appFramework->Init();
    }
    else
    {
        MR_LOG(LogApplication, Log, "Shutting down Application!");

        windowManager->Shutdown();

        layerManager->Shutdown();
        
        Logger::Shutdown();

        exit(exitCode);
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
