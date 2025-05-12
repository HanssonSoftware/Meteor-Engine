/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WinWindowManager.h"
#include <../Resources/resource.h>
#include <Core/Application.h>
#include "WinWindow.h"
#include <RHI/RHILoader.h>
#include <RHI/RHIRegistry.h>
#include <Layers/OSLayer.h>

IWindow* WindowsWindowManager::CreateNativeWindow(const WindowCreateInfo* CreateInfo)
{
	if (CreateInfo->windowID.IsEmpty())
	{
		MR_LOG(LogWindowManager, Error, TEXT("CreateInfo->windowID is Empty!"));
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
    const HWND focused = GetFocus();

    return SearchForHWND(focused);
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

    case WM_SIZE:
        //if (Application::Get()->GetAppState() == APPLICATIONSTATE_RUNNING && Application::Get()->GetRenderContext()->getDeviceReadyState() == GRAPHICS_ENGINE_STATE_RUNNING)
        //{
        //    uint32 width = LOWORD(p2);
        //    uint32 height = HIWORD(p2);

        //    Application::Get()->GetRenderContext()->resizeBuffers(Vector2(width, height));
        //}
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
        // OutputDebugString(L"Close\n");
        Application::Get()->SetAppState(APPLICATIONSTATE_SHUTDOWN);
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
            //auto temp = Application::Get()->GetWindowManager()->GetRenderContext();
            //temp->setIsFullScreen(!temp->getIsFullScreen());
        }
        if (GetAsyncKeyState(0x41) & 0x01)
        {
            Application::Get()->SetAppState(APPLICATIONSTATE_RESTARTING);
        }
        if (GetAsyncKeyState(0x42) & 0x01)
        {/*
            IDirect3DDevice* temp = (IDirect3DDevice*)Application::Get()->GetWindowManager()->GetRenderContext();
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
