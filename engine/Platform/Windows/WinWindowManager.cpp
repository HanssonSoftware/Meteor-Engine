/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WinWindowManager.h"
#include <Log/Exception.h>
#include <../Resources/resource.h>
#include <Application/Application.h>
#include "WinWindow.h"


IWindow* WinWindowManager::createWindow(const WindowCreateInfo* CreateInfo)
{
	if (CreateInfo->windowID.isEmpty())
	{
		MR_LOG(LogWindowManager, Error, TEXT("CreateInfo->windowID is Empty!"));
		return nullptr;
	}

	if (!bIsWinAPIClassRegistered)
	{
		if (!registerWindowClass())
		{
			THROW_EXCEPTION("Failed to Register Window Class!");
		}
	}

	WinWindow* newWindow = new WinWindow(this);
	if (newWindow->createWindow(CreateInfo))
	{
		activeWindows.push_back(newWindow);

		if (getFirstWindow() == newWindow)
		{
			renderContext = new IDirect3DDevice();
			renderContext->Init();
		}

		return newWindow;
	}
	else
	{
		THROW_EXCEPTION("Failed to Create Window!");
	}

	return nullptr;
}

WinWindowManager::~WinWindowManager()
{
	WindowManager::~WindowManager();

	UnregisterClass(ApplicationClassName, GetModuleHandle(NULL));
}

inline bool WinWindowManager::registerWindowClass()
{
	HINSTANCE instance = GetModuleHandle(NULL);
	HICON ico = (HICON)LoadImage(instance, MAKEINTRESOURCE(IDI_DEFAULTAPPICON), IMAGE_ICON, 64, 64, LR_DEFAULTCOLOR);

	WNDCLASSEX windowClass = {};
	windowClass.lpszClassName = ApplicationClassName;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hInstance = instance;
	windowClass.hIcon = ico;
	windowClass.hIconSm = ico;
	windowClass.lpfnWndProc = MeteorSpecifiedWindowProcedure;

	if (!RegisterClassEx(&windowClass))
	{
		THROW_EXCEPTION("Failed to register WinAPI class!");
		return false;
	}

	bIsWinAPIClassRegistered = true;
	return true;
}

IWindow* WinWindowManager::getFocusedWindow() const
{
    return nullptr;
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
