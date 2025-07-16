/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WindowsWindowManager.h"
#include <../Resources/resource.h>
#include <Core/Application.h>
#include "WindowsWindow.h"
#include <RHI/RHILoader.h>
#include <RHI/RHIRegistry.h>
#include <Layers/SystemLayer.h>
#include <dwmapi.h>

#pragma comment (lib, "UxTheme.lib")

void WindowsWindowManager::Init()
{
    IWindowManager::Init();
}

void WindowsWindowManager::Shutdown()
{
    IWindowManager::Shutdown();

    if (bIsWinAPIClassRegistered)
    {
        if (!bIsUsingFallbackClassName)
        {
            if (const Application* app = Application::Get())
            {
                UnregisterClass(Layer::GetSystemLayer()->ConvertToWide(app->GetAppInfo().appName.Chr()), instance);
            }
        }
        else
        {
            UnregisterClass(ApplicationClassName, instance);
        }
    }
}

IWindow* WindowsWindowManager::CreateNativeWindow(const WindowCreateInfo* CreateInfo)
{
	if (CreateInfo->windowID.IsEmpty())
	{
		MR_LOG(LogWindowManager, Error, "CreateInfo->windowID is Empty!");
		return nullptr;
	}

	if (!bIsWinAPIClassRegistered)
	{
		if (!RegisterWindowClass())
		{
            MR_LOG(LogWindowManager, Fatal, "Failed to register window class!");
            return nullptr;
		}
	}

	WindowsWindow* newWindow = new WindowsWindow(this);
	if (newWindow->CreateNativeWindow(CreateInfo))
	{
		activeWindows.push_back(newWindow);

		if (GetFirstWindow() == newWindow)
		{
            renderContext = RHILoader::InitRHI(SYSTEM_LAYOUT_WINDOWS);
            if (!renderContext->Initialize())
            {
                MR_LOG(LogWindowManager, Fatal, "Failed to create rendering interface! %s", renderContext->GetName().Chr());
                return nullptr; // just in case
            }

            newWindow->ShowWindow();
		}

		return newWindow;
	}
	else
	{
        MR_LOG(LogWindowManager, Error, "Failed to create window!");
	}

	return nullptr;
}

WindowsWindow* WindowsWindowManager::SearchForHWND(const HWND hWnd)
{
    for (IWindow* win : activeWindows)
    {
        if (WindowsWindow* wina = (WindowsWindow*)win)
        {
            if ((HWND)wina->GetWindowHandle() == hWnd)
            {
                return wina;
            }
        }
    }

    return nullptr;
}


WindowsWindowManager::~WindowsWindowManager()
{

}

inline bool WindowsWindowManager::RegisterWindowClass()
{
	instance = GetModuleHandle(NULL);
	HICON ico = (HICON)LoadImage(instance, MAKEINTRESOURCE(IDI_DEFAULTAPPICON), IMAGE_ICON, 64, 64, LR_DEFAULTCOLOR);

	WNDCLASSEX windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hInstance = instance;
	windowClass.hIcon = ico;
	windowClass.hIconSm = ico;
	windowClass.lpfnWndProc = MeteorSpecifiedWindowProcedure;

    if (const Application* app = Application::Get())
    {
        windowClass.lpszClassName = Layer::GetSystemLayer()->ConvertToWide(app->GetAppInfo().appName.Chr());
    }
    else
    {
        MR_LOG(LogWindowManager, Error, "Failed to get application codename, using fallback name! Multiple instance detection may not work!");
        bIsUsingFallbackClassName = true;
        windowClass.lpszClassName = ApplicationClassName;
    }

	if (!RegisterClassEx(&windowClass))
	{
		MR_LOG(LogWindowManager, Fatal, "Failed to register WinAPI class!");
		return false;
	}

	bIsWinAPIClassRegistered = true;
	return true;
}

IWindow* WindowsWindowManager::GetFocusedWindow()
{
    return SearchForHWND(GetFocus());
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
        if (WindowsWindowManager* wm = (WindowsWindowManager*)Application::Get()->GetWindowManager())
        {
            if (WindowsWindow* wind = wm->SearchForHWND(wnd))
            {
                //wind->DestroyWindow();
            }
        }

        break;

    case WM_CLOSE:
        App::RequestExit(0);
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
