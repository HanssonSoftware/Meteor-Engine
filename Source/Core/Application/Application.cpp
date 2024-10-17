/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#include "Application.h"
#include <Window/Window.h>
#include <Graphics/D3D11/DX11.h>
#include <thread>
#include <Common/MemoryManager.h>
#include <Widgets/Viewport.h>
//#include <Log/Log.h>

Application* Application::Framework;

Application::Application(const String Name, Vector2<uint32> Size)
	: applicationName(Name)
	, windowSize(Size)
{
    Framework = this;
    Init();
}

void Application::Frame()
{
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
    Logger::Get().FirstStartLogger();
    MR_LOG(LogApplication, Log, TEXT("Initializing application."));
    

	Window* Win = new Window(getApplicationName().Chr(), windowSize, nullptr);
    setWindowHandle(Win->getWindowHandle());

    Viewport& ViewportSystem = getSubsystem<Viewport>();

    IDX11Device& D3D11Device = getSubsystem<IDX11Device>();
    std::thread D3Thread = std::thread(&IDX11Device::init, &D3D11Device);

    //createLayer(D3D11);

    // If we use Debug mode, append the current RHI signature to the window title.
#ifdef MR_DEBUG
    wchar_t Temp[512];
    swprintf_s(Temp, 512, L"%s <%s>", getApplicationName().Chr(), D3D11Device.getRendererSignatature().c_str());
    setApplicationName(Temp);
#endif // MR_DEBUG

	MSG msg;
    bIsAppRunning = true;
    while (bIsAppRunning)
    {
        const auto calculatedNow = std::chrono::high_resolution_clock::now();

        if (PeekMessage(&msg, Win->getWindowHandle(), 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        D3D11Device.render();

        Frame();

        auto calculated = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - calculatedNow);

        //if (calculated < frameDuration)
            //std::this_thread::sleep_for(frameDuration - calculated);
    }

    MR_LOG(LogApplication, Log, TEXT("Shutting down application!"));

    D3D11Device.cleanUp();
    D3Thread.join();

    Logger::Get().shutdownLogger();
	delete Win;
}

void Application::setAppInfo(const ApplicationInitializationInfo* appInfo)
{
    setApplicationName(appInfo->appName);
    setWindowSize(appInfo->windowCreateInfo->size);

    this->appInfo = appInfo;
}

Application::~Application()
{
}

void* Application::getWindowHandle()
{
    if (!windowHandle)
        windowHandle = (HWND)GetActiveWindow();

    return windowHandle;
}

void Application::setApplicationName(const String Name)
{
    applicationName = Name;

#ifdef _WIN32
    SetWindowText((HWND)getWindowHandle(), *applicationName);
#endif // _WIN32
}

void Application::drawAttention()
{
#ifdef _WIN32
    FLASHWINFO flashInfo = {};
    flashInfo.cbSize = sizeof(flashInfo);
    flashInfo.hwnd = (HWND)getWindowHandle();
    flashInfo.dwFlags = FLASHW_TRAY;
    flashInfo.uCount = 0;
    flashInfo.dwTimeout = 0;

    FlashWindowEx(&flashInfo);
#endif // _WIN32
}

LRESULT CALLBACK WndProc(HWND wnd, UINT uint, WPARAM p1, LPARAM p2)
{
    switch (uint)
    {
    case WM_CREATE:
         // OutputDebugString(L"Create\n");
        break;

    //case WM_PAINT:
    //    break;

    case WM_SIZE:
        if (Application::Get().bIsAppRunning)
        {
            uint32 width = LOWORD(p2);
            uint32 height = HIWORD(p2);

            auto& DeviceSystem = getSubsystem<IDX11Device>();
            DeviceSystem.updateSize(Vector2(width, height));
        }
        break;

    case WM_DESTROY:
         // OutputDebugString(L"Destroy\n");
        PostQuitMessage(97);
        // Clean up window-specific data objects.
        break;

    case WM_CLOSE:
         // OutputDebugString(L"Close\n");
        Application::Get().bIsAppRunning = false;
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
            auto& temp = getSubsystem<IDX11Device>();
            temp.setIsFullScreen(!temp.getIsFullScreen());
        }
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


