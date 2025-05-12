/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Application.h"
#include <Platform/Window.h>
#include <RHI/RHIOutputContext.h>
#include <RHI/RHIRegistry.h>
#include <thread>

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

static Application* appFramework;

Application::Application(const ApplicationInitializationInfo& aInfo)
	: appInfo(aInfo)
{
    appFramework = this;
    //applicationLocation = ICommandlet::Get().executableLocation;

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
    static std::mutex mtx;
    std::lock_guard<std::mutex> l(mtx);
    return appFramework ? appFramework : nullptr;
}

void Application::Init()
{
#ifdef MR_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif // MR_DEBUG

    layerManager->Init();

    logger.Intialize();
    MR_LOG(LogApplication, Log, TEXT("Initializing Application."));

    if (appInfo.appName.IsEmpty())
    {
        MR_LOG(LogApplication, Fatal, TEXT("AppInfo is Invalid!"));
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

		//if (graphicsDevice) graphicsDevice->GetOutputContext()->Clear();
		//if (graphicsDevice) graphicsDevice->GetOutputContext()->Draw();

        tm.Stop();

        double t = tm.Evaluate();
        if (layerManager) layerManager->UpdateLayer();

        //SceneGraph::Get().Update(0.01f);
        Framework->Run();
    }

    //if (windowManager->GetRenderContext() != nullptr)
    //{
    //    if (graphicsDevice->renderThread.joinable())
    //        graphicsDevice->renderThread.join();
    //}


    //windowManager->DestroyWindow("Super");
    Framework->Shutdown();
}

void Application::Shutdown()
{
    if (GetAppState() == APPLICATIONSTATE_RESTARTING)
    {
        MR_LOG(LogApplication, Log, TEXT("Restarting Application!"));
    
        Framework->Init();
    }
    else
    {
        MR_LOG(LogApplication, Log, TEXT("Shutting down Application!"));

        windowManager->Destroy();

        layerManager->Destroy();

        //ILogger::Get().shutdownLogger();

        exit(0);
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
    if (appInfo.appName.IsEmpty())
    {
        return GetWindowManager()->GetFirstWindow()->windowData.windowName;
    }

    return appInfo.appName;
}


inline void Application::CreateNativeWindow() const
{
    if (appInfo.flags & APPFLAG_NO_WINDOW)
        return;

    GetWindowManager()->CreateNativeWindow(&appInfo.windowCreateInfo);
}
