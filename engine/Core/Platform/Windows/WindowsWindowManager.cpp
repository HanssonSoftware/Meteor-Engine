/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "WindowsWindowManager.h"
#include <../Resources/resource.h>
#include <Core/Application.h>
#include "WindowsWindow.h"
#include <Layers/SystemLayer.h>
#include <dwmapi.h>
#include <Module/ModuleManager.h>

#pragma comment (lib, "UxTheme.lib")

void WindowsWindowManager::Init()
{
    IWindowManager::Init();

}

WindowsWindowManager::~WindowsWindowManager() noexcept
{
    IWindowManager::~IWindowManager();
}


void WindowsWindowManager::Shutdown()
{
    IWindowManager::Shutdown();

    if (bIsWinAPIClassRegistered)
    {
        if (!bIsUsingFallbackClassName)
        {
            if (const Application* app = GetApplication())
            {
                UnregisterClassW(Layer::GetSystemLayer()->ConvertToWide(app->appNameNoSpaces.Chr()), instance);
            }
        }
        else
        {
            UnregisterClassW(GetDefaultApplicationName(), instance);
        }
    }
}

bool WindowsWindowManager::Present()
{
    return false;
}

bool WindowsWindowManager::CreateWindow(const String& name, const Vector2<uint32_t> size)
{
    if (size < 8)

    WindowCreateInfo& aWindowData = this->windowData;
    aWindowData = *windowData;

    HINSTANCE Inst = GetModuleHandle(0);

    RECT windowRect = { 0, 0, (LONG)windowData->size.x, (LONG)windowData->size.y };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, 0);
    aWindowData.windowName = windowData->windowName;

    const wchar_t* buffer = Layer::GetSystemLayer()->ConvertToWide(aWindowData.windowName.Chr());

    const wchar_t* className = nullptr;
    bool bIsFallback = false;
    if (const Application* app = Application::Get())
    {
        if (const WindowsWindowManager* wm = (WindowsWindowManager*)owner)
        {
            bIsFallback = wm->GetIsUsingFallbackClass();
            className = bIsFallback ? GetDefaultApplicationName() : Layer::GetSystemLayer()->ConvertToWide(app->appNameNoSpaces.Chr());
        }
    }

    handle = ::CreateWindowExW(
        /*WS_EX_ACCEPTFILES*/ 0,
        className,
        buffer,
        windowData->flags == -1 ? WS_OVERLAPPEDWINDOW : windowData->flags,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        0,
        0,
        Inst,
        0
    );

    if (!bIsFallback) delete[] className;

    SetWindowTextW(handle, buffer);
    delete[] buffer;
    if (handle == INVALID_HANDLE_VALUE)
    {
        MR_LOG(LogWindowManager, Error, "Failed to create WinAPI window!");
        return false;
    }


    static const constexpr BOOL bCanIUseDarkWindowTitlebar = 1;
    DwmSetWindowAttribute((HWND)handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &bCanIUseDarkWindowTitlebar, sizeof(bCanIUseDarkWindowTitlebar));

}

IWindow* WindowsWindowManager::FindHWNDCorresponding(const HWND hWnd)
{
    for (IWindow* window : activeWindows)
    {
        if ((HWND)window->GetWindowHandle() == hWnd)
            return window;
    }

    return nullptr;
}

inline bool WindowsWindowManager::RegisterWindowClass()
{
	instance = GetModuleHandle(nullptr);
	HICON ico = (HICON)LoadImage(instance, MAKEINTRESOURCE(IDI_DEFAULTAPPICON), IMAGE_ICON, 64, 64, LR_DEFAULTCOLOR);

	WNDCLASSEXW windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hInstance = instance;
	windowClass.hIcon = ico;
	windowClass.hIconSm = ico;
	windowClass.lpfnWndProc = MeteorSpecifiedWindowProcedure;

    if (const Application* app = Application::Get())
    {
        windowClass.lpszClassName = Layer::GetSystemLayer()->ConvertToWide(app->appNameNoSpaces.Chr());
    }
    else
    {
        MR_LOG(LogWindowManager, Error, "Failed to get application codename, using fallback name! Multiple instance detection may not work!");
        bIsUsingFallbackClassName = true;
        windowClass.lpszClassName = GetDefaultApplicationName();
    }

	if (!RegisterClassExW(&windowClass))
	{
		MR_LOG(LogWindowManager, Fatal, "Failed to register WinAPI class!");
		return false;
	}

	bIsWinAPIClassRegistered = true;
	return true;
}

IWindow* WindowsWindowManager::GetFocusedWindow()
{
    return FindHWNDCorresponding(GetFocus());
}

LRESULT CALLBACK MeteorSpecifiedWindowProcedure(HWND wnd, UINT uint, WPARAM p1, LPARAM p2)
{
    switch (uint)
    {
    case WM_CREATE:
        // OutputDebugString(L"Create\n");
        break;

        //case WM_PAINT:
        //    break;

    case WM_SYSCOMMAND:
        break;

    case WM_SIZE:
        break;

    case WM_DESTROY:
        
        break;

    case WM_CLOSE:
        Application::RequestExit(0);
        break;
    case WM_SIZING:
        break;

    case WM_WINDOWPOSCHANGING:
        break;

    case WM_WINDOWPOSCHANGED:
        break;
    case WM_KEYDOWN:
        break;
    case WM_MOVING:
        break;
    default:
        return DefWindowProc(wnd, uint, p1, p2);
    }

    return DefWindowProc(wnd, uint, p1, p2);
}
