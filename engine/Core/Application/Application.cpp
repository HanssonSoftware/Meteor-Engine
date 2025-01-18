/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Application.h"
#include <Window/Window.h>
#include <Graphics/D3D11/Direct3DDevice.h>
#include <thread>
#include <Common/MemoryManager.h>
#include <Widgets/Viewport.h>
#include <Application/LayerManager.h>
//#include <imgui.h>
//#include <backends/imgui_impl_dx11.h>
//#include <backends/imgui_impl_win32.h>
#include <mutex>
#include "Commandlet.h"

#ifdef MR_DEBUG
#include <crtdbg.h>
#endif // MR_DEBUG


Application* Application::Framework;

Application::Application(const ApplicationInitializationInfo* aInfo)
	: appInfo(aInfo)
{
    Framework = this;
    applicationLocation = ICommandlet::Get().executableLocation;

    setWindowManager(new WindowManager());
    setLayerManager(new LayerManager());
}

void Application::Frame(float deltaTime)
{
}

Application* Application::Get()
{
    static std::mutex mtx;
    std::lock_guard<std::mutex> l(mtx);
    return Framework ? Framework : nullptr;
}

uint32 targetedFramesPerSecond = 60;
const std::chrono::milliseconds frameDuration(1000 / targetedFramesPerSecond);
void Application::Init()
{
#ifdef MR_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif // MR_DEBUG

    Logger::Get().firstStartLogger();
    MR_LOG(LogApplication, Log, TEXT("Initializing Application."));

    applicationLocation = ICommandlet::Get().executableLocation;
    MR_LOG(LogApplication, Verbose, TEXT("App Dir: %s"), applicationLocation.Chr());

    if (appInfo->appName.isEmpty())
    {
        MR_LOG(LogApplication, Fatal, TEXT("AppInfo is Bad, or Null!"));
    }

    instantiateWindow();
    
    if (Application::Get()->getAppInfo()->flags & APPFLAG_DISPLAY_QUICK_INFO_ABOUT_MEMORY_USAGE)
        MemoryManager::Get().bQuickMemoryLogging = true;


    if (getRenderContext())
    {
        getRenderContext()->graphToRender = &SceneGraph::Get();
        //getRenderContext()->setImGUIUsed(true);
    }


    setAppState(APPLICATIONSTATE_RUNNING);
}

void Application::Run()
{
    IGraphicsDevice* graphicsDevice = (IGraphicsDevice*)getWindowManager()->getRenderContext();
    Window* Ref = getWindowManager()->searchFor("Super");

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

    SceneGraph::Get().addToRoot(&nc);

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

        Frame(dt);
        Framework->Run();
    }

    if (getWindowManager()->getRenderContext() != nullptr)
    {
        if (graphicsDevice->renderThread.joinable())
            graphicsDevice->renderThread.join();
    }


    WindowManager::Get().destroyWindow("Super");
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

void Application::setAppInfo(const ApplicationInitializationInfo* appInfo)
{
    this->appInfo = appInfo;
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
    if (!appInfo)
    {
        MR_LOG(LogApplication, Error, TEXT("AppInfo is Null!"));
        return String();
    }

    if (appInfo->appName.isEmpty())
    {
        return getWindowManager()->getFirstWindow()->windowData->windowName;
    }

    return appInfo->appName;
}

Vector2<uint32> Application::getWindowSize() const
{
    static Vector2<uint32> lastResort;

    if (lastResort != 0)
        return lastResort;

    lastResort = getWindowManager()->getFirstWindow()->getSize();

    return lastResort;
}

void Application::drawAttention() const
{
    if (getAppState() == APPLICATIONSTATE_INITIALIZATION)
        return;

    if (!getWindowManager()->getFirstWindow())
        return;

#ifdef _WIN32
    FLASHWINFO flashInfo = {};
    flashInfo.cbSize = sizeof(flashInfo);
    flashInfo.hwnd = (HWND)getWindowManager()->getFirstWindow()->getWindowHandle();
    flashInfo.dwFlags = FLASHW_TRAY;
    flashInfo.uCount = 0;
    flashInfo.dwTimeout = 0;

    FlashWindowEx(&flashInfo);
#endif // _WIN32
}

inline void Application::instantiateWindow()
{
    if (appInfo->flags & APPFLAG_NO_WINDOW)
        return;

    if (appInfo->windowCreateInfo != nullptr)
    {
        getWindowManager()->createWindow(appInfo->windowCreateInfo);
    }
    else
    {
        MR_LOG(LogWindowManager, Log, TEXT("appInfo->windowCreateInfo is Null! Perhaps you didn't Need a Window?"));
    }
}

LRESULT CALLBACK WndProc(HWND wnd, UINT uint, WPARAM p1, LPARAM p2)
{
    //if (ImGui_ImplWin32_WndProcHandler(wnd, uint, p1, p2))
    //    return true;

    if (Logger::Get().getMessageName(uint) != L"" && false) {
        MR_LOG(LogApplication, Verbose, TEXT("%s"), Logger::Get().getMessageName(uint));
    }

    switch (uint)
    {
    case WM_CREATE:
         // OutputDebugString(L"Create\n");
        break;

    //case WM_PAINT:
    //    break;

    case WM_SIZE:
        if (Application::Get()->getAppState() == APPLICATIONSTATE_RUNNING && Application::Get()->getRenderContext()->getDeviceReadyState() == GRAPHICS_ENGINE_STATE_RUNNING)
        {
            uint32 width = LOWORD(p2);
            uint32 height = HIWORD(p2);

            Application::Get()->getRenderContext()->resizeBuffers(Vector2(width, height));
        }
        break;

    case WM_DESTROY:
        //WindowManager::Get().destroyWindow();
        // Clean up window-specific data objects.
        break;

    case WM_CLOSE:
         // OutputDebugString(L"Close\n");
        Application::Get()->setAppState(APPLICATIONSTATE_SHUTDOWN);
        break;
    
    case WM_SIZING:
        break;  

    case WM_WINDOWPOSCHANGING:
        break;    
    
    case WM_WINDOWPOSCHANGED:
        break;

    //case WM_QUIT:
    //     // OutputDebugString(L"Quit\n");
    //    break;

    //case WM_ACTIVATE:
    //     // OutputDebugString(L"Activate\n");
    //    break;

    case WM_KEYDOWN:
        if (GetAsyncKeyState(VK_TAB) & 0x01)
        {
            auto temp = Application::Get()->getWindowManager()->getRenderContext();
            temp->setIsFullScreen(!temp->getIsFullScreen());
        }        
        if (GetAsyncKeyState(0x41) & 0x01)
        {
            Application::Get()->setAppState(APPLICATIONSTATE_RESTARTING);
        }       
        if (GetAsyncKeyState(0x42) & 0x01)
        {/*
            IDirect3DDevice* temp = (IDirect3DDevice*)Application::Get()->getWindowManager()->getRenderContext();
            temp->changeDisplayMode();*/
        }
        break;
    case WM_MOVING:

        break;
    //case WM_KEYUP:
    //     // OutputDebugString(L"KeyUp\n");
    //    break;

    //case WM_LBUTTONDOWN:
    //     // OutputDebugString(L"LeftButtonDown\n");
    //    break;

    //case WM_LBUTTONUP:
    //     // OutputDebugString(L"LeftButtonUp\n");
    //    break;

    //case WM_RBUTTONDOWN:
    //     // OutputDebugString(L"RightButtonDown\n");
    //    break;

    //case WM_RBUTTONUP:
    //     // OutputDebugString(L"RightButtonUp\n");
    //    break;

    //case WM_MOUSEMOVE:
    //     // OutputDebugString(L"MouseMove\n");
    //    break;

    //case WM_MOUSEWHEEL:
    //     // OutputDebugString(L"MouseWheel\n");
    //    break;

    //case WM_SYSKEYDOWN:
    //     // OutputDebugString(L"SysKeyDown\n");
    //    break;

    //case WM_SYSKEYUP:
    //     // OutputDebugString(L"SysKeyUp\n");
    //    break;

    ///* Sent to a window if the mouse causes the cursor to move within a window and mouse input is not captured. */
    ////case WM_SETCURSOR:
    ////     // OutputDebugString(L"SetCursor\n");
    ////    break;

    //case WM_CHAR:
    //     // OutputDebugString(L"Char\n");
    //    break;

    //case WM_KILLFOCUS:
    //     // OutputDebugString(L"KillFocus\n");
    //    break;
    default:
        return DefWindowProc(wnd, uint, p1, p2);
    }

    return DefWindowProc(wnd, uint, p1, p2);
}

