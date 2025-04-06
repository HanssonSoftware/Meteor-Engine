/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Application.h"
#include <Window/Window.h>
#include <D3D11/Direct3DDevice.h>
#include <thread>

#include <Common/MemoryManager.h>
#include <Widgets/Viewport.h>
#include <Layers/LayerManager.h>
#include <GraphicsEngine/SceneGraph.h>
#include <mutex>
#include "Commandlet.h"
#ifdef _WIN32
#include <VersionHelpers.h>
#endif // MR_DEBUG
#ifdef MR_DEBUG
#include <crtdbg.h>
#endif // MR_DEBUG
#ifdef _WIN32

#endif // MR_DEBUG

Application* Application::Framework;

Application::Application(const ApplicationInitializationInfo& aInfo)
	: appInfo(aInfo)
{
    Framework = this;
    applicationLocation = ICommandlet::Get().executableLocation;

#ifdef _WIN32
    WinWindowManager* wm = new WinWindowManager();
#endif // _WIN32


    setWindowManager();
    setLayerManager(new LayerManager());
}

Application* Application::Get()
{
    static std::mutex mtx;
    std::lock_guard<std::mutex> l(mtx);
    return Framework ? Framework : nullptr;
}

void Application::Init()
{
#ifdef MR_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif // MR_DEBUG

    checkIfAnotherInstanceIsRunning();

    Logger::Get().firstStartLogger();
    MR_LOG(LogApplication, Log, TEXT("Initializing Application."));

#ifdef _WIN32
    if (!IsWindows7OrGreater())
    {
        MR_LOG(LogApplication, Fatal, TEXT("This Windows Version is Not Supported!"));
    }
#endif // _WIN32

    //MR_LOG(LogApplication, Verbose, TEXT("App Dir: %s"), applicationLocation.Chr());

    if (appInfo.appName.isEmpty())
    {
        MR_LOG(LogApplication, Fatal, TEXT("AppInfo is Invalid!"));
    }

    instantiateWindow();
    
    if (Application::Get()->appInfo.flags & APPFLAG_DISPLAY_QUICK_INFO_ABOUT_MEMORY_USAGE)
        MemoryManager::Get().bQuickMemoryLogging = true;


    if (getRenderContext())
    {
        SceneGraph::Get().addToRoot(&nc);
        //getRenderContext()->graphToRender = &SceneGraph::Get();
        //getRenderContext()->setImGUIUsed(true);
    }


    setAppState(APPLICATIONSTATE_RUNNING);
}

void Application::Run()
{
    IGraphicsDevice* graphicsDevice = (IGraphicsDevice*)getWindowManager()->getRenderContext();
    IWindow* Ref = getWindowManager()->searchFor("Super");

    std::atomic<float> dt = 0.f;

    HWND windowReference = (HWND)Ref->getWindowHandle();

  /*  if (getWindowManager()->getRenderContext() != nullptr)
        graphicsDevice->renderThread = std::thread([&graphicsDevice]() 
            { 
                while (Application::Get()->getAppState() == ApplicationState::Running)
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

    Ref->showWindow();

    setAppState(APPLICATIONSTATE_RUNNING);

    MSG msg;
    while (getAppState() == APPLICATIONSTATE_RUNNING)
    {
        const auto calculatedNow = std::chrono::high_resolution_clock::now();

        if (PeekMessage(&msg, windowReference, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        std::chrono::duration<float> calculated = (std::chrono::high_resolution_clock::now() - calculatedNow);
        dt = calculated.count();

        if (graphicsDevice->getDeviceReadyState() == GRAPHICS_ENGINE_STATE_RUNNING)
        {
            graphicsDevice->Render();
        }

        SceneGraph::Get().Update(dt);
        Framework->Run();
    }

    if (windowManager->getRenderContext() != nullptr)
    {
        if (graphicsDevice->renderThread.joinable())
            graphicsDevice->renderThread.join();
    }


    windowManager->destroyWindow("Super");
    Framework->Shutdown();
}

void Application::Shutdown()
{
    if (getAppState() == APPLICATIONSTATE_RESTARTING)
    {
        MR_LOG(LogApplication, Log, TEXT("Restarting Application!"));
    
        Framework->Init();
    }
    else
    {
        MR_LOG(LogApplication, Log, TEXT("Shutting down Application!"));

        windowManager->Destroy();

        layerManager->Destroy();

        Logger::Get().shutdownLogger();
    }
}

Application::~Application()
{
    if (windowManager)
        windowManager = nullptr;
    
    if (layerManager)
        layerManager = nullptr;
}

String Application::getApplicationName()
{
    if (appInfo.appName.isEmpty())
    {
        return getWindowManager()->getFirstWindow()->windowData.windowName;
    }

    return appInfo.appName;
}

Vector2<uint32> Application::getWindowSize() const
{
    static Vector2<uint32> lastResort;

    if (lastResort != 0)
        return lastResort;

    lastResort = getWindowManager()->getFirstWindow()->getSize();

    return lastResort;
}

inline void Application::instantiateWindow() const
{
    if (appInfo.flags & APPFLAG_NO_WINDOW)
        return;

    getWindowManager()->createWindow(&appInfo.windowCreateInfo);
}

inline void Application::checkIfAnotherInstanceIsRunning()
{
#ifdef _WIN32
    HANDLE fakeMutex = CreateMutexW(0, 0, /*applicationCodeName.Chr()*/L"long_dick");
    switch (GetLastError())
    {
    case ERROR_ALREADY_EXISTS:
    {
        const int selected = MessageBoxW(0, L"anyad", L"fa", MB_OK);
        exit(-5);
        break;
    }   
    case ERROR_SUCCESS:
    {
        break;
    }
    default:
        break;
    }

    if (fakeMutex) CloseHandle(fakeMutex);
#endif // _WIN32
}
