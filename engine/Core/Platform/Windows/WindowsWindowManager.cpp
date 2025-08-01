﻿/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "WindowsWindowManager.h"
#include <../Resources/resource.h>
#include <Core/Application.h>
#include "WindowsWindow.h"
#include <Layers/SystemLayer.h>
#include <dwmapi.h>
#include <Renderer/Registry.h>

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
            if (const Application* app = GetApplication())
            {
                UnregisterClassW(Layer::GetSystemLayer()->ConvertToWide(app->GetAppInfo().appName.Chr()), instance);
            }
        }
        else
        {
            UnregisterClassW(ApplicationClassName, instance);
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
            if (!VulkanRegistry::Initialize())
            {
                MR_LOG(LogWindowManager, Fatal, "Failed to create rendering interface! %s", *VulkanRegistry::GetName());
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
