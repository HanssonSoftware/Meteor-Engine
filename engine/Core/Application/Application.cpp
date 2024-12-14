/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Application.h"
#include <Window/Window.h>
#include <Graphics/D3D11/Direct3DDevice.h>
#include <thread>
#include <Common/MemoryManager.h>
#include <Widgets/Viewport.h>
#include <Window/WindowManager.h>
#include <Graphics/MeshModel.h>
#include <Graphics/Vulkan/VulkanDevice.h>
#include <Application/LayerManager.h>
#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include <Log/Log.h>
#include <Log/Exception.h>
#include "Commandlet.h"

Application* Application::Framework;

Application::Application(const ApplicationInitializationInfo* aInfo)
	: appInfo(aInfo)
{
    setWindowManager(new WindowManager());
    setLayerManager(new LayerManager());

}

void Application::Frame(float deltaTime)
{
}

Application* Application::Get()
{
    return Framework ? Framework : nullptr;
}

String Application::getExecutableDir() const
{
#ifdef _WIN32
    wchar_t Temp[2048];
    GetModuleFileNameW(NULL, Temp, 2048);
    return String(Temp);
#endif // _WIN32

}

uint32 targetedFramesPerSecond = 60;
const std::chrono::milliseconds frameDuration(1000 / targetedFramesPerSecond);
void Application::Init()
{
    Logger::Get().firstStartLogger();
    MR_LOG(LogApplication, Log, TEXT("Initializing application."));
    Logger::Get().printCollectedLogs();
  
    getWindowManager()->createWindow(appInfo->windowCreateInfo);


    // If we use Debug mode, append the current RHI signature to the window title.
#ifdef MR_DEBUG
    //initApplicationDebugMode(getWindowManager()->getRenderContext());
#else
    setApplicationName(getApplicationName());
#endif // MR_DEBUG

    getWindowManager()->getRenderContext()->graphToRender = &SceneGraph::Get();
    getWindowManager()->getRenderContext()->setImGUIUsed(true);


    //Viewport* ViewportSystem = Viewport();
    if (appInfo->appName.isEmpty())
    {
        MR_LOG(LogApplication, Fatal, TEXT("AppInfo is bad, or null!"));
    }

    setAppState(Running);
}

//void Application::initApplicationDebugMode(const IGraphicsDevice* selectedRHI)
//{
//    String appName = getApplicationName();
//    const String rhiName = selectedRHI->getRendererSignatature();
//
//    if (appInfo->windowCreateInfo)
//    {
//        if (!appInfo->windowCreateInfo->windowName.isEmpty()) 
//            appName = appInfo->windowCreateInfo->windowName;
//    }
//
//
//    if (selectedRHI == nullptr)
//    {
//        MR_LOG(LogApplication, Error, TEXT("Unable to append RHI's name to the title. Perhaps not initialized?"));
//#ifdef _WIN32
//        SetWindowText((HWND)Application::Get()->getWindowManager()->getFirstWindow()->getWindowHandle(), getApplicationName().Chr());
//#endif // _WIN32
//        return;
//    }
//
//    const size_t calculated = swprintf(NULL, 0, L"%s <%s>", appName.Chr(), rhiName.Chr());
//
//    wchar_t* superBuffer = (wchar_t*)mrmalloc((calculated + 1) * sizeof(wchar_t));
//
//    swprintf(superBuffer, calculated + 1, L"%s <%s>", appName.Chr(), rhiName.Chr());
//
//#ifdef _WIN32
//    SetWindowText((HWND)Application::Get()->getWindowManager()->getFirstWindow()->getWindowHandle(), superBuffer);
//#endif // _WIN32
//
//    mrfree(superBuffer);
//}

void Application::Run()
{
    Window* Ref = getWindowManager()->searchFor("Super");

    IDirect3DDevice* D3DRef = (IDirect3DDevice*)getWindowManager()->getRenderContext();
    std::atomic<float> dt = 0.f;

    HWND A = (HWND)Ref->getWindowHandle();
    std::thread D3Thread = std::thread([&D3DRef, &dt]() { D3DRef->Render(dt); });

    Ref->showWindow();

    SceneGraph::Get().addToRoot(&nc);

    MSG msg;
    setAppState(Running);

    while (getAppState() == EApplicationState::Running)
    {
        const auto calculatedNow = std::chrono::high_resolution_clock::now();

        if (PeekMessage(&msg, A, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        std::chrono::duration<float> calculated = (std::chrono::high_resolution_clock::now() - calculatedNow);
        dt = calculated.count();

        D3DRef->Render(dt);
        SceneGraph::Get().Update(dt);

        Frame(dt);
        Framework->Run();
    }

    D3Thread.join();

    WindowManager::Get().destroyWindow("Super");
    Framework->Shutdown();
}

void Application::Shutdown()
{
    if (getAppState() == EApplicationState::Restarting)
    {
        MR_LOG(LogApplication, Log, TEXT("Restarting application!"));
    
        Framework->Init();
    }
    else
    {
        MR_LOG(LogApplication, Log, TEXT("Shutting down application!"));

        windowManager->Destroy();

        layerManager->Destroy();

        Logger::Get().shutdownLogger();
    }

    //auto a = MemoryManager::Get().getTotalUsedMemory();
    //auto b = MemoryManager::Get().getUsedMemory();

    //int j = 5;
}

void Application::setAppInfo(const ApplicationInitializationInfo* appInfo)
{
    this->appInfo = appInfo;

    setApplicationName(appInfo->appName);
}

Application::~Application()
{
    if (windowManager)
    {
        windowManager = nullptr;
    }   
    
    if (layerManager)
    {
        windowManager = nullptr;
    }
}


void Application::setApplicationName(const String Name)
{
    applicationName = Name;

    if (/*true == nullptr*/true)
        return;

#ifdef _WIN32
    SetWindowText((HWND)getWindowManager()->getFirstWindow()->getWindowHandle(), *applicationName);
#endif // _WIN32
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
        setApplicationName(getWindowManager()->getFirstWindow()->windowData->windowName);
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
    if (getAppState() == EApplicationState::Initialization)
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

LRESULT CALLBACK WndProc(HWND wnd, UINT uint, WPARAM p1, LPARAM p2)
{
    if (ImGui_ImplWin32_WndProcHandler(wnd, uint, p1, p2))
        return true;

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
        if (Application::Get()->getAppState() == Running)
        {
            uint32 width = LOWORD(p2);
            uint32 height = HIWORD(p2);

            Application::Get()->getWindowManager()->getRenderContext()->resizeBuffers(Vector2(width, height));
        }
        break;

    case WM_DESTROY:
        //WindowManager::Get().destroyWindow();
        // Clean up window-specific data objects.
        break;

    case WM_CLOSE:
         // OutputDebugString(L"Close\n");
        Application::Get()->setAppState(Shutdown);
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
            Application::Get()->setAppState(EApplicationState::Restarting);
        }       
        if (GetAsyncKeyState(0x42) & 0x01)
        {
            IDirect3DDevice* temp = (IDirect3DDevice*)Application::Get()->getWindowManager()->getRenderContext();
            temp->changeDisplayMode();
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


